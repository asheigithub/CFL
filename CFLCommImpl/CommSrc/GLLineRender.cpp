#include "headers/CFLLineRender.h"
#include "CFLDrawCommand.h"
#include "CFLEntry.h"

#include "buildinshaders/GLLineShader.h"
#include "headers/CFLVertBufferTemplate.h"

namespace cfl
{

	namespace graphic
	{
		using namespace cfl::render;

		
		struct lineVec
		{
			GLfloat x;
			GLfloat y;

			GLubyte r;
			GLubyte g;
			GLubyte b;
			GLubyte a;

		};


		

		class lineVertBuffer :public VertBuffer<lineVec>
		{

		};
		
		

		GLLineRender::GLLineRender(render::RenderThreadLooper* looper, Graphic* g) :
			IGLLineRender(RenderType::drawline, looper,g),
			buff(new lineVertBuffer()),
			//buff2(new lineVertBuffer()),
			vbo(nullptr),
			progDrawLine(nullptr),
			lastState(new lineDrawState())
		{
			
			buff->datasource = std::make_shared<VecDataSource<lineVec>>(VecDataSource<lineVec>());;
			buff->datasource->setData(buff);
		};

		GLLineRender::~GLLineRender()
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

		

		void GLLineRender::onGLinited(CFLContext* ctx)
		{

			

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

		void GLLineRender::buildShader(CFLContext* ctx)
		{

			auto vs = ctx->glObjCollection->createShader();
			
			vs->compile(gl_line_shader_vert, ShaderType::vertex_shader);

			auto fs = ctx->glObjCollection->createShader();
			
			fs->compile(gl_line_shader_frag, ShaderType::fragment_shader);

			progDrawLine = ctx->glObjCollection->createProgram();
			progDrawLine->link(vs, fs, true);


		}
		
		void GLLineRender::swapRenderQueue(CFLContext* ctx)
		{
			
			//更新VBO
			if (vbo == nullptr && buff->datasource->getByteLength()>0)
			{
				//LOGI("bufflen: %d\n", buff->datasource->getByteLength());


				vbo = CFLContext::getCurrentInstance()->glObjCollection->createVBO();
				vbo->genBuffer(vbotarget::gl_array_buffer, vbousage::gl_dynamic_draw,
					buff->datasource->getByteLength() , buff->datasource, 0,true,false
					);

			}
			else if (vbo !=nullptr && vbo->isValid() )
			{
				doBuffSubData(vbo, 0, buff->datasource->getByteLength(), buff->datasource, 0, false);

			}

			
			buff->reset();
			
		}


		void GLLineRender::drawLine(const geom::Vector2& stpos, const geom::Vector2& edpos,const Color& color, float linewidth)
		{
			
			linewidth = math::clampf(linewidth, lineWidthRange.min, lineWidthRange.max);

			if (progDrawLine == nullptr)
			{
				//在被调用时再创建shader
				buildShader( CFLContext::getCurrentInstance() );
			}

			auto allocret = buff->getVects(2);

			lineVec* verts = allocret.ptr;
			verts[0].x = stpos.x;
			verts[0].y = stpos.y;

			verts[0].r = static_cast<GLubyte>(255 * color.r);
			verts[0].g = static_cast<GLubyte>(255 * color.g);
			verts[0].b = static_cast<GLubyte>(255 * color.b);
			verts[0].a = static_cast<GLubyte>(255 * color.a);


			verts[1].x = edpos.x;
			verts[1].y = edpos.y;

			verts[1].r = static_cast<GLubyte>(255 * color.r);
			verts[1].g = static_cast<GLubyte>(255 * color.g);
			verts[1].b = static_cast<GLubyte>(255 * color.b);
			verts[1].a = static_cast<GLubyte>(255 * color.a);

			size_t* nums = new size_t(2);
			lastState->drawNumsPtr = nums; //保存地址
			lastState->line_width = linewidth;

			auto buffoffset = allocret.offset;

			
			auto fun = [this, nums, buffoffset,linewidth]()
			{
				glLineWidth(linewidth);

				//glUseProgram(progDrawLine->getHandle());
				graphic->getGLProxy()->useProgram(progDrawLine);


				auto color = progDrawLine->getAttribute("line_color")->location;
				auto index = progDrawLine->getAttribute("a_position")->location;

				glBindBuffer(vbo->getTarget(), vbo->getHandle());

				glVertexAttribPointer(index
					, 2, GL_FLOAT, GL_FALSE, sizeof(lineVec), 
					
					(void*)(buffoffset * sizeof(lineVec) ));

				
				
				glVertexAttribPointer(color
					, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(lineVec),
					
					(void*)((offsetof(lineVec, r) + buffoffset* sizeof(lineVec))  ));


				
				glEnableVertexAttribArray(index);
				glEnableVertexAttribArray(color);

				glDrawArrays(GL_LINES, 0, *nums);

				glDisableVertexAttribArray(index);
				glDisableVertexAttribArray(color);
				
				glBindBuffer(vbo->getTarget(), 0);

				
				delete nums;

			};

			auto command = new funcDrawCommand< decltype(fun)>(fun, "drawLine");

			renderLooper->pushCommand(command);
		}

		void GLLineRender::appendLine(const geom::Vector2& stpos, const geom::Vector2& edpos,const Color& color, float linewidth)
		{
			linewidth = math::clampf(linewidth, lineWidthRange.min, lineWidthRange.max);
			if (math::abs(linewidth- lastState->line_width) > 0.1f)
			{
				drawLine(stpos, edpos,color, linewidth);
				return;
			}

			auto allocret = buff->getVects(2);

			lineVec* verts = allocret.ptr;
			verts[0].x = stpos.x;
			verts[0].y = stpos.y;

			verts[0].r = static_cast<GLubyte>(255 * color.r);
			verts[0].g = static_cast<GLubyte>(255 * color.g);
			verts[0].b = static_cast<GLubyte>(255 * color.b);
			verts[0].a = static_cast<GLubyte>(255 * color.a);


			verts[1].x = edpos.x;
			verts[1].y = edpos.y;

			verts[1].r = static_cast<GLubyte>(255 * color.r);
			verts[1].g = static_cast<GLubyte>(255 * color.g);
			verts[1].b = static_cast<GLubyte>(255 * color.b);
			verts[1].a = static_cast<GLubyte>(255 * color.a);

			*lastState->drawNumsPtr += 2;
			

		}

	}
}