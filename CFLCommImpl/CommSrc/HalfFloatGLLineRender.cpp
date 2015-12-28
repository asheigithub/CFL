#include "headers/CFLLineRender.h"
#include "CFLDrawCommand.h"
#include "CFLEntry.h"

#include "buildinshaders/GLLineShader.h"
#include "CFLHALF_FLOAT.h"


#include "GLES2/gl2ext.h"
#include "headers/CFLVertBufferTemplate.h"

namespace cfl
{

	namespace graphic
	{
		using namespace cfl::render;

		
		struct half_lineVec
		{
			FLOAT16 x;
			FLOAT16 y;

			GLubyte r;
			GLubyte g;
			GLubyte b;
			GLubyte a;

		};

		class halfFloatlineVertBuffer : public VertBuffer<half_lineVec>
		{

		};


		HalfFloatGLLineRender::HalfFloatGLLineRender(render::RenderThreadLooper* looper, Graphic* g) :
			IGLLineRender(RenderType::drawline, looper,g),
			buff(new halfFloatlineVertBuffer()),
			//buff2(new lineVertBuffer()),
			vbo(nullptr),
			progDrawLine(nullptr),
			lastState(new lineDrawState())
		{

			buff->datasource = std::make_shared<VecDataSource<half_lineVec>>(VecDataSource<half_lineVec>());;
			buff->datasource->setData(buff);
		};

		HalfFloatGLLineRender::~HalfFloatGLLineRender()
		{
			if (vbo)
			{
				CFLContext::getCurrentInstance()->glObjCollection->deleteVBO(vbo);
			}

			if (progDrawLine)
			{
				CFLContext::getCurrentInstance()->glObjCollection->deleteProgram(progDrawLine);
			}
			delete lastState;
			delete buff;//, delete buff2;
		}

		static decltype(GL_HALF_FLOAT) GL_HF_TYPE = GL_HALF_FLOAT;

		void HalfFloatGLLineRender::onGLinited(CFLContext* ctx)
		{

			if (ctx->capability->getGlVer() < 3)
			{
				GL_HF_TYPE = GL_HALF_FLOAT_OES;
			}

			{
				//***查询画线宽度***
				GLfloat* linewidthstore = lineWidthRange.range;

				auto queryGLlineWidth = [linewidthstore]()
				{
					glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, linewidthstore);
					CFL_CHECKGLERROR;
				};

				auto command = new funcDrawCommand< decltype(queryGLlineWidth)>(queryGLlineWidth, "queryGLlineWidth");
				renderLooper->pushCommand(command);

			}


		}

		void HalfFloatGLLineRender::buildShader(CFLContext* ctx)
		{

			auto vs = ctx->glObjCollection->createShader();

			vs->compile(gl_line_shader_vert, ShaderType::vertex_shader);

			auto fs = ctx->glObjCollection->createShader();

			fs->compile(gl_line_shader_frag, ShaderType::fragment_shader);

			progDrawLine = ctx->glObjCollection->createProgram();
			progDrawLine->link(vs, fs, true);


		}

		void HalfFloatGLLineRender::swapRenderQueue(CFLContext* ctx)
		{
			
			//更新VBO
			if (vbo == nullptr && buff->datasource->getByteLength()>0)
			{
				//LOGI("bufflen: %d\n", buff->datasource->getByteLength());


				vbo = CFLContext::getCurrentInstance()->glObjCollection->createVBO();
				vbo->genBuffer(vbotarget::gl_array_buffer, vbousage::gl_dynamic_draw,
					buff->datasource->getByteLength(), buff->datasource, 0, true, false
					);

			}
			else if (vbo != nullptr && vbo->isValid())
			{
				doBuffSubData(vbo, 0, buff->datasource->getByteLength(), buff->datasource, 0, false);

			}


			buff->reset();

		}


		void HalfFloatGLLineRender::drawLine(const geom::Vector2& stpos, const geom::Vector2& edpos, const Color& color, float linewidth)
		{

			linewidth = math::clampf(linewidth, lineWidthRange.min, lineWidthRange.max);

			if (progDrawLine == nullptr)
			{
				//在被调用时再创建shader
				buildShader(CFLContext::getCurrentInstance());
			}

			auto allocret = buff->getVects(2);

			half_lineVec* verts = allocret.ptr;
			verts[0].x = (stpos.x);
			verts[0].y = (stpos.y);

			verts[0].r = static_cast<GLubyte>(255 * color.r);
			verts[0].g = static_cast<GLubyte>(255 * color.g);
			verts[0].b = static_cast<GLubyte>(255 * color.b);
			verts[0].a = static_cast<GLubyte>(255 * color.a);


			verts[1].x = (edpos.x);
			verts[1].y = (edpos.y);

			verts[1].r = static_cast<GLubyte>(255 * color.r);
			verts[1].g = static_cast<GLubyte>(255 * color.g);
			verts[1].b = static_cast<GLubyte>(255 * color.b);
			verts[1].a = static_cast<GLubyte>(255 * color.a);

			size_t* nums = new size_t(2);
			lastState->drawNumsPtr = nums; //保存地址
			lastState->line_width = linewidth;

			auto buffoffset = allocret.offset;

			
			auto fun = [this, nums, buffoffset, linewidth]()
			{
				glLineWidth(linewidth);

				//glUseProgram(progDrawLine->getHandle());
				graphic->getGLProxy()->useProgram(progDrawLine);


				auto color = progDrawLine->getAttribute("line_color")->location;
				auto index = progDrawLine->getAttribute("a_position")->location;

				glBindBuffer(vbo->getTarget(), vbo->getHandle());

				glVertexAttribPointer(index
					, 2, GL_HF_TYPE, GL_FALSE, sizeof(half_lineVec),
					
					(void*)(buffoffset * sizeof(half_lineVec)));



				glVertexAttribPointer(color
					, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(half_lineVec),
					
					(void*)((offsetof(half_lineVec, r) + buffoffset* sizeof(half_lineVec))));



				glEnableVertexAttribArray(index);
				glEnableVertexAttribArray(color);

				glDrawArrays(GL_LINES, 0, *nums);

				glDisableVertexAttribArray(index);
				glDisableVertexAttribArray(color);

				glBindBuffer(GL_ARRAY_BUFFER, 0);


				delete nums;

			};

			auto command = new funcDrawCommand< decltype(fun)>(fun, "drawLine");

			renderLooper->pushCommand(command);
		}

		void HalfFloatGLLineRender::appendLine(const geom::Vector2& stpos, const geom::Vector2& edpos, const Color& color, float linewidth)
		{
			linewidth = math::clampf(linewidth, lineWidthRange.min, lineWidthRange.max);
			if (math::abs(linewidth - lastState->line_width) > 0.1f)
			{
				drawLine(stpos, edpos, color, linewidth);
				return;
			}

			auto allocret = buff->getVects(2);

			half_lineVec* verts = allocret.ptr;
			verts[0].x = stpos.x;
			verts[0].y = stpos.y;

			


			verts[0].r = static_cast<GLubyte>(255 * color.r);
			verts[0].g = static_cast<GLubyte>(255 * color.g);
			verts[0].b = static_cast<GLubyte>(255 * color.b);
			verts[0].a = static_cast<GLubyte>(255 * color.a);


			verts[1].x = (edpos.x);
			verts[1].y = (edpos.y);

			

			verts[1].r = static_cast<GLubyte>(255 * color.r);
			verts[1].g = static_cast<GLubyte>(255 * color.g);
			verts[1].b = static_cast<GLubyte>(255 * color.b);
			verts[1].a = static_cast<GLubyte>(255 * color.a);

			*lastState->drawNumsPtr += 2;


		}

	}
}