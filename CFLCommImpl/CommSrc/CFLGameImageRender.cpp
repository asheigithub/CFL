#include "headers/CFLGameImageRender.h"
#include "headers/CFLGameImageRender_DrawElement.h"

#include "buildinshaders/GameImage_TinyColor_Shader.h"
#include "buildinshaders/GameImage_Separate_TinyColor_Shader.h"
#include "buildinshaders/GameImage_Atlas_TinyColor.h"

#include "CFLEntry.h"

#include "CFLDrawCommand.h"
#include "headers/CFLVertBufferTemplate.h"
#include "Graphic/CFLGLVBO.h"

namespace cfl
{
	namespace graphic
	{
		struct gameimagevert
		{
			float x;
			float y;
			float z;
			float u;
			float v;

			GLubyte r;
			GLubyte g;
			GLubyte b;
			GLubyte a;
		};

		static VertBuffer<gameimagevert>* vertBuffer=nullptr;
		static VertBuffer<GLushort>* indexBuffer = nullptr;


		static int currentdrawindex ;
		static int _drawbatchCounter ;

		
		static float _fdx=0.0f;
		static float _fdy=0.0f;

		//2D绘图的MVP矩阵
		static geom::Matrix3D mvp;

		static std::vector<drawElement*> draws;//缓存绘图信息列表
		
		//计算此次绘图的屏幕坐标
		static bool calDrawScreenMatrix(drawElement* d);

		//将绘图信息插入绘图列表
		static void insertdrawintodraws(
			drawElement* draw
			);
		//检查两个draw信息之间是否兼容
		static bool checkchange( drawElement* d1,drawElement* d2 );

		static void updateMVP(cfl::Stage* stage)
		{
			mvp.identity();
			
			//变换进世界空间
			mvp.append(
				geom::Matrix3D(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-(float)stage->stageWidth() / 2, (float)stage->stageHeight() / 2, 0.0f, 1.0f));


			//放大2倍，正好进视口
			mvp.appendScale(2.0f, 2.0f, 1.0f);

			auto lookat = geom::Matrix3D::lookAtLH(0, 0, -(float)stage->stageWidth() * 2, 0, 0, 0, 0, 1, 0);
			mvp.append(lookat);

			auto perspect = geom::Matrix3D::perspectiveOffCenterLH(
				-(float)stage->stageWidth() / 2, (float)stage->stageWidth() / 2,
				-(float)stage->stageHeight() / 2, (float)stage->stageHeight() / 2, (float)stage->stageWidth(), (float)stage->stageWidth() + (float)stage->stageWidth() * 2);

			perspect.append(geom::Matrix3D(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 2.0f, 0.0f,
				0.0f, 0.0f, -1.0f, 1.0f)); //将z转到-1,1之间

			mvp.append(perspect);

		}

		static render::GLProgram* build(GameImageRender* grender, const char* vert, const char* frag, render::GLProgram*& progloc)
		{
			if (progloc == nullptr)
			{
				auto vs = CFLContext::getCurrentInstance()->glObjCollection->createShader();
				vs->compile(vert, render::ShaderType::vertex_shader);

				auto fs = CFLContext::getCurrentInstance()->glObjCollection->createShader();
				fs->compile(frag, render::ShaderType::fragment_shader);

				progloc = CFLContext::getCurrentInstance()->glObjCollection->createProgram();
				progloc->link(vs, fs, true);
			}
			return progloc;
		}

		//static render::GLProgram* build_tinycolor(GameImageRender* grender)
		//{
		//	return build(grender, gameimage_tinycolor_vert, gameimage_tinycolor_frag, 
		//		(grender->gameimage_tinycolor));
		//}

		//static render::GLProgram* build_atlas_tinycolor(GameImageRender* grender)
		//{
		//	return build(grender, gameimage_atlas_tinycolor_vert, gameimage_atlas_tinycolor_frag,
		//		(grender->gameimage_tinycolor_atlas));
		//}

		//static render::GLProgram* build_separate_tinycolor(GameImageRender* grender)
		//{
		//	return build(grender, gameimage_separate_tinycolor_vert, gameimage_separate_tinycolor_frag,
		//		(grender->gameimage_tinycolor_separate));

		//}


		////选择着色器
		//static render::GLProgram* selectProgram(GameImageRender* grender, Texture2DRef* refTex2D )
		//{
		//	auto alphatype = refTex2D->getAlphaType();

		//	switch (alphatype)
		//	{
		//	case cfl::content::alphachannal:
		//		return build_tinycolor(grender); //grender->gameimage_tinycolor;
		//		break;
		//	case cfl::content::atlas:
		//		return build_atlas_tinycolor(grender); //grender->gameimage_tinycolor_atlas;
		//		break;
		//	case cfl::content::separate:
		//		return build_separate_tinycolor(grender); //grender->gameimage_tinycolor_separate;
		//		break;
		//	default:
		//		return build_tinycolor(grender);  //grender->gameimage_tinycolor;
		//		break;
		//	}
		//}
		//static void setProgramArgSp(const Graphic* graphic, 
		//	render::GLProgram* program,
		//	GameImageRender* grender, Texture2DRef* refTex2D,const GameImage* image)
		//{
		//	if (program == grender->gameimage_tinycolor_separate)
		//	{
		//		auto alphaLoc = program->getUniform("a_texture")->location;
		//		//绑alpha纹理//
		//		glActiveTexture(GL_TEXTURE1);
		//		glBindTexture(GL_TEXTURE_2D, refTex2D->getSeparateTexture()->getHandle());
		//		glUniform1i(alphaLoc, 1);
		//	}
		//}


		GameImageRender::GameImageRender(render::RenderThreadLooper* looper, Graphic* g)
			:IGameImageRender(RenderType::gameimage,looper,g),
			/*gameimage_tinycolor(nullptr), 
			gameimage_tinycolor_atlas(nullptr),
			gameimage_tinycolor_separate(nullptr),*/
			indexVbo(nullptr), 
			vertVbo(nullptr),
			createdPrograms()
		{
			vertBuffer = new VertBuffer<gameimagevert>();
			vertBuffer->datasource = std::make_shared< VecDataSource<gameimagevert> >();
			vertBuffer->datasource->setData(vertBuffer);

			indexBuffer = new VertBuffer<GLushort>();
			indexBuffer->datasource = std::make_shared< VecDataSource<GLushort>>();
			indexBuffer->datasource->setData(indexBuffer);

		}

		GameImageRender::~GameImageRender()
		{
			/*if (gameimage_tinycolor)
			{
				CFLContext::getCurrentInstance()->glObjCollection->deleteProgram(gameimage_tinycolor);
			}
			if (gameimage_tinycolor_separate)
			{
				CFLContext::getCurrentInstance()->glObjCollection->deleteProgram(gameimage_tinycolor_separate);
			}
			if (gameimage_tinycolor_atlas)
			{
				CFLContext::getCurrentInstance()->glObjCollection->deleteProgram(gameimage_tinycolor_atlas);
			}*/

			for (auto i = createdPrograms.begin(); i != createdPrograms.end(); i++)
			{
				auto prog = *i;
				CFLContext::getCurrentInstance()->glObjCollection->deleteProgram(*prog);
			}
			

			if (vertVbo)
			{
				CFLContext::getCurrentInstance()->glObjCollection->deleteVBO(vertVbo);
			}
			if (indexVbo)
			{
				CFLContext::getCurrentInstance()->glObjCollection->deleteVBO(indexVbo);
			}
			if (vertBuffer)
			{
				delete vertBuffer;
				vertBuffer = nullptr;
			}
			if (indexBuffer)
			{
				delete indexBuffer;
				indexBuffer = nullptr;
			}
		}

		void GameImageRender::onGLinited(CFLContext* ctx)
		{
			buildShader(ctx);

			currentdrawindex = 0;
			_drawbatchCounter = 0;

		}

		void GameImageRender::buildShader(CFLContext* ctx)
		{
			//**需要时再创建**

		}

		void GameImageRender::deActive(CFLContext* ctx) //当被其他绘图命令替代时
		{
			callBatch();
		}

		void GameImageRender::swapRenderQueue(CFLContext* ctx)//通知渲染队列即将交换在此时更新VBO
		{
			callBatch();
			
			//设置VBO数据

			if (vertVbo == nullptr && vertBuffer->datasource->getByteLength() > 0)
			{
				vertVbo = CFLContext::getCurrentInstance()->glObjCollection->createVBO();
				vertVbo->genBuffer(render::vbotarget::gl_array_buffer, render::vbousage::gl_dynamic_draw,
					vertBuffer->datasource->getByteLength(), vertBuffer->datasource, 0, true, false
					);
			}
			else if (vertVbo != nullptr && vertVbo->isValid())
			{
				cfl::render::doBuffSubData(vertVbo, 0,
					vertBuffer->datasource->getByteLength(), vertBuffer->datasource, 0, false);

			}

			auto idxused = indexBuffer->getUsed() / 6;
			auto vertused = vertBuffer->getUsed() / 4;
			if (idxused<vertused )
			{
				auto idxpos = idxused;
				auto indexAlloc = indexBuffer->getVects((vertused - idxused) * 6);
				GLushort* indices = indexAlloc.ptr;  //new GLubyte[(ed - st) * 6];

				for (size_t j = 0; j < vertused-idxused; j++, idxpos++)
				{
					indices[j * 6 + 0] = idxpos * 4;
					indices[j * 6 + 1] = idxpos * 4 + 1;
					indices[j * 6 + 2] = idxpos * 4 + 2;
					indices[j * 6 + 3] = idxpos * 4 + 3;

					indices[j * 6 + 4] = idxpos * 4 + 3; //退化三角形
					indices[j * 6 + 5] = idxpos * 4 + 4;
				}
			}


			if (indexVbo == nullptr && indexBuffer->datasource->getByteLength()>0)
			{
				
				indexVbo = CFLContext::getCurrentInstance()->glObjCollection->createVBO();
				indexVbo->genBuffer(render::vbotarget::gl_element_array_buffer, render::vbousage::gl_static_draw,
					indexBuffer->datasource->getByteLength(), indexBuffer->datasource, 0, true, false
					);

			}
			else if (indexVbo != nullptr && indexVbo->isValid() && idxused<vertused)
			{
				render::doBuffSubData(indexVbo, 0,
					indexBuffer->datasource->getByteLength(), indexBuffer->datasource, 0, false);

			}

			drawElement::reset();

			vertBuffer->reset();
			
			//**更新mvp***
			updateMVP(ctx->stage);

		}
		
		
		

		void GameImageRender::buildDrawCommand(int st, int ed)
		{
			
			auto vertAlloc = vertBuffer->getVects((ed - st) * 4);
			gameimagevert* verts =vertAlloc.ptr;  //new gameimagevert[(ed - st)*4];


			for (int i = 0 , pos=st ; i < ed-st; i++,pos++)
			{
				auto d = *draws[pos];

				auto alpha = d.opacity * d.a;

				verts[i*4].x = d.p1.x;
				verts[i * 4].y = d.p1.y;
				verts[i * 4].z = d.p1.z;
				verts[i * 4].u = d._leftu;
				verts[i * 4].v = d._topv;
				verts[i * 4].r = static_cast<GLubyte>(255 * d.r);
				verts[i * 4].g = static_cast<GLubyte>(255 * d.g);
				verts[i * 4].b = static_cast<GLubyte>(255 * d.b);
				verts[i * 4].a = static_cast<GLubyte>(255 * alpha);


				verts[i * 4+1].x = d.p3.x;
				verts[i * 4 + 1].y = d.p3.y;
				verts[i * 4 + 1].z = d.p3.z;
				verts[i * 4 + 1].u = d._leftu;
				verts[i * 4 + 1].v = d._bottomv;
				verts[i * 4 + 1].r = static_cast<GLubyte>(255 * d.r);
				verts[i * 4 + 1].g = static_cast<GLubyte>(255 * d.g);
				verts[i * 4 + 1].b = static_cast<GLubyte>(255 * d.b);
				verts[i * 4 + 1].a = static_cast<GLubyte>(255 * alpha);

				verts[i * 4 + 2].x = d.p2.x;
				verts[i * 4 + 2].y = d.p2.y;
				verts[i * 4 + 2].z = d.p2.z;
				verts[i * 4 + 2].u = d._rightu;
				verts[i * 4 + 2].v = d._topv;
				verts[i * 4 + 2].r = static_cast<GLubyte>(255 * d.r);
				verts[i * 4 + 2].g = static_cast<GLubyte>(255 * d.g);
				verts[i * 4 + 2].b = static_cast<GLubyte>(255 * d.b);
				verts[i * 4 + 2].a = static_cast<GLubyte>(255 * alpha);

				verts[i * 4 + 3].x = d.p4.x;
				verts[i * 4 + 3].y = d.p4.y;
				verts[i * 4 + 3].z = d.p4.z;
				verts[i * 4 + 3].u = d._rightu;
				verts[i * 4 + 3].v = d._bottomv;
				verts[i * 4 + 3].r = static_cast<GLubyte>(255 * d.r);
				verts[i * 4 + 3].g = static_cast<GLubyte>(255 * d.g);
				verts[i * 4 + 3].b = static_cast<GLubyte>(255 * d.b);
				verts[i * 4 + 3].a = static_cast<GLubyte>(255 * alpha);
			}
			
			auto num = ed - st;
			auto image = draws[st]->image;
			
			auto effect = draws[st]->effect; //draws[st]->effectData.effect;
			//auto dx = draws[st]->effectData.dx;
			draws[st]->effect = nullptr;


			auto program = effect->getProgram(
				image->refTexture.get(),
				createdPrograms
				); //selectProgram(this, image->refTexture.get());
			
			auto voffset = vertAlloc.offset;
			
			auto fun = [this, num, voffset, image, program, effect]()
			{
				
				//混合模式
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				
				
				//2D,反正也不用背面剔除...
				//if (d.matrix && ( math::unequalf(d.matrix->M02, 0.0f)
				//	|| math::unequalf(d.matrix->M12, 0.0f)
				//	|| math::unequalf(d.matrix->M22, 1.0f)
				//	)
				//	)
				//{//3d旋转，不能背面剔除
					glDisable(GL_CULL_FACE);
				//}
				//else
				//{ 
				//	//裁剪
				//	glEnable(GL_CULL_FACE);
				//	glFrontFace(GL_CCW);
				//	glCullFace(GL_BACK);
				//}
				
				graphic->getGLProxy()->useProgram(program);
				//glUseProgram(gameimage_tinycolor->getHandle());
				
				//各个着色器都有的属性
				auto color = program->getAttribute("color")->location;
				

				auto positon = program->getAttribute("vPosition")->location;
				auto uv = program->getAttribute("uv")->location;
				
				auto matrixidx = program->getUniform("vp_matrix")->location;
				
				auto samplerLoc = program->getUniform("s_texture")->location;
				
				//绑纹理//
				glActiveTexture(GL_TEXTURE0);

				
				glBindTexture(GL_TEXTURE_2D, image->refTexture->getTexture2D()->getHandle());
				
				
				glUniform1i(samplerLoc, 0);
				
				//各着色器特殊参数
				//setProgramArgSp(graphic,program, this, image->refTexture.get(),&image);
				effect->setProgramArgs(graphic, program, image->refTexture.get(), &image);
				

				glBindBuffer(vertVbo->getTarget(), vertVbo->getHandle());

				glVertexAttribPointer(positon
					, 3, GL_FLOAT, GL_FALSE, sizeof(gameimagevert), 
					(void*)(voffset * sizeof(gameimagevert)) );
				glVertexAttribPointer(uv
					, 2, GL_FLOAT, GL_FALSE, sizeof(gameimagevert), 
					(void*)(voffset * sizeof(gameimagevert) + offsetof(gameimagevert, u)));
				glVertexAttribPointer(color
					, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(gameimagevert), 
					(void*)((voffset * sizeof(gameimagevert)) + offsetof(gameimagevert, r)));


				glUniformMatrix4fv(matrixidx,1,GL_FALSE, &mvp.M00 );

				glEnableVertexAttribArray(positon);
				glEnableVertexAttribArray(uv);
				glEnableVertexAttribArray(color);

				
				glBindBuffer(indexVbo->getTarget(), indexVbo->getHandle());
				glDrawElements(GL_TRIANGLE_STRIP, 6 * num -2 , GL_UNSIGNED_SHORT, 0);


				glDisableVertexAttribArray(positon);
				glDisableVertexAttribArray(uv);
				glDisableVertexAttribArray(color);

				glBindBuffer(vertVbo->getTarget(), 0);
				glBindBuffer(indexVbo->getTarget(), 0);

				

				
			};
			

			auto command = new render::funcDrawCommand< decltype(fun)>(fun, "drawGameImage");
			renderLooper->pushCommand(command);

		}


		//将累计的绘图信息推送给GPU
		void GameImageRender::callBatch()
		{
			int stidx  = 0;

			int bcount  = 0;
			int count  = 1;
			for (int i  = 0; i < currentdrawindex; i++)
			{
				if (i == currentdrawindex - 1 
					/*|| count == 24   //现在没有数量限制 */  
					|| (draws[i]->__group != draws[i + 1]->__group) )
				{

					//CallRenderNoBatch(s1, s2, stidx, i + 1);
					buildDrawCommand(stidx, i + 1);
					
					stidx = i + 1;

					bcount++;
					count = 1;

				}
				else
				{
					count += 1;
				}
			}

			currentdrawindex = 0;
			_drawbatchCounter++;
		}





		void GameImageRender::drawGameImage(
			bool isAppend,//标记上次调用是否也是drawGameImage

			const GameImage& image,
			float tx,
			float ty,
			const cfl::geom::Matrix3D* matrix,
			float opacity ,
			const cfl::geom::Rectangle* clip ,
			float scale ,
			float roation ,
			const Color* color ,
			FilterMode filter ,
			BlendMode blendmode,
			std::shared_ptr<IGameImageEffect> effect
			)
		{

			
			auto d = drawElement::getDrawElement();
			if (color)
			{
				d->r = color->r;
				d->g = color->g;
				d->b = color->b;
				d->a = color->a;
			}
			else
			{
				d->r = 1.0f; d->g = 1.0f; d->b = 1.0f; d->a = 1.0f;
			}

			if (clip != nullptr)
			{
				d->clip = &d->backclip;
				*d->clip = *clip;
			}
			else
			{
				d->clip = nullptr;
			}
			
			d->image = image;
			if (matrix != nullptr)
			{
				d->matrix = &d->backmatrix;
				*d->matrix = *matrix;
				
			}
			else
			{
				d->matrix = nullptr;
			}

			d->blendmode = blendmode;
			d->opacity = opacity;
			d->roation = roation;
			d->scale = scale;
			d->tx = tx;
			d->ty = ty;
			d->filtermode = filter;
			

			d->effect = effect;

			if (!isAppend)
			{
				//先组织之前累计的绘图队列
				callBatch();
			}

			insertdrawintodraws(d);


		}
		//将绘图信息插入绘图列表
		void insertdrawintodraws(
			drawElement* draw
			)
		{
			if (currentdrawindex >= (long)draws.size())
			{
				for (size_t i = 0; i < 1024; i++)
				{
					draws.push_back(nullptr);
				}
			}

			auto reftextrue = draw->image->refTexture;

			if (reftextrue->isFailed())
			{
				//**纹理有错误**
			}
			else if (reftextrue->isDone() || reftextrue->isValid())
			{
				
				{

					if (!calDrawScreenMatrix(draw))
					{
						return;
					}

					int idx  = currentdrawindex;

					int foundinset = -1;

					int toskipgroup = -1;


					if (reftextrue->__useinbatchCounter() != _drawbatchCounter)  //该纹理此次被初次使用
					{
						reftextrue->__useinbatchCounter(_drawbatchCounter);
						reftextrue->__useinbatchStIdx(currentdrawindex);
					}
					else
					{
						int batchstidx = reftextrue->__useinbatchStIdx(); 
						while (idx>batchstidx )
						{
							bool needcheck = false; 
							drawElement* checkd  = draws[idx - 1];
							if (toskipgroup != checkd->__group)
							{
								toskipgroup = checkd->__group;
								needcheck = true;
							}

							if (needcheck && !checkchange(checkd, draw)) //找到插入点
							{
								foundinset = idx;

								draw->__group = checkd->__group;

								if (foundinset == currentdrawindex) //如果上一个和当前可以是一组，就直接归到上一组。
								{
									break;
								}
							}
							
							
							if ((
								checkd->_screenright <= draw->_screenleft || 
								checkd->_screenleft >= draw->_screenright  ||
								checkd->_screentop >= draw->_screenbottom || 
								checkd->_screenbottom <= draw->_screentop))
							{
								//***和之前不相交，可以前移

								idx--;
							}
							else
							{
								break;
							}

						}


					}
					if (foundinset > 0)
					{
						//trace(foundinset);

						if (foundinset < currentdrawindex)
						{
							for (int j  = currentdrawindex - 1; j >= foundinset; j--)
							{
								draws[j + 1] = draws[j];
							}
						}
						draws[foundinset] = draw;

						

					}
					else
					{
						draw->__group = currentdrawindex;
						draws[currentdrawindex] = draw;

						

					}



				}

				++currentdrawindex;

			}
			else
			{
				//**纹理尚未加载完成**
			}
		}

		//计算此次绘图的屏幕坐标
		static bool calDrawScreenMatrix(drawElement* d)
		{
			GameImage& img  = d->image;
			//*****计算矩阵**********
			geom::Matrix3D m;
			
			float leftu  = img->leftTopU;
			float rightu = img->rightBottomU;

			float topv  = img->leftTopV;
			float bottomv = img->rightBottomV;

			//***恢复到原图像*****
			if (d->clip == nullptr)
			{
				m.M[0][0] = (float)img->clipWidth;
				m.M[1][1] = (float)img->clipHeight;

				m.M[3][0] = (float)img->padLeft;
				m.M[3][1] = (float)img->padTop;

				/*m.appendScale((float)img->clipWidth, (float)img->clipHeight, 1.0f);
				m.appendTranslation((float)img->padLeft, (float)img->padTop, 0.0f);*/

			}
			else
			{

				//m.appendTranslation(d.clip.left, d.clip.top , 0);
				int dx  = 0;
				int dy  = 0;
				int il  = 0;
				if (d->clip->getLeft() < img->padLeft)
				{
					il = img->padLeft;

					dx = (int)( img->padLeft - d->clip->getLeft());
				}
				else
				{
					il = (int)(d->clip->getLeft());
				}

				int it  = 0;
				if (d->clip->getTop() < img->padTop)
				{
					it = img->padTop;

					dy = (int)(img->padTop - d->clip->getTop());
				}
				else
				{
					it = (int)(d->clip->getTop());
				}
				int ir =0;
				if (d->clip->getRight() >= img->padLeft + img->clipWidth)
				{
					ir = (int)(img->padLeft + img->clipWidth);
				}
				else
				{
					ir = (int)(d->clip->getRight());
				}

				int ib  = 0;
				if (d->clip->getBottom() >= img->padTop + img->clipHeight)
				{
					ib = img->padTop + img->clipHeight;
				}
				else
				{
					ib = (int)(d->clip->getBottom());
				}

				if (ir - il > 0 && ib - it > 0)
				{
					m.appendScale((float)(ir - il), (float)(ib - it), 1.0f);
				}
				else
				{
					return false;
				}
				m.appendTranslation((float)dx, (float)dy, 0.0f);

				float newleftu  = leftu + (il - img->padLeft) * (rightu - leftu) / img->clipWidth;
				float newtopv   = topv + (it - img->padTop) * (bottomv - topv) / img->clipHeight;

				float newrightu  = leftu + (ir - img->padLeft) * (rightu - leftu) / img->clipWidth;
				float newbottomv  = topv + (ib - img->padTop) * (bottomv - topv) / img->clipHeight;

				leftu = newleftu;
				topv = newtopv;
				rightu = newrightu;
				bottomv = newbottomv;
			}
			
			m.appendScale(d->scale, d->scale, 1);
			
			
			if (d->matrix != nullptr)
			{
				m.append( *d->matrix );
			}
			if (math::unequalf(d->roation, 0.0f))
			{
				m.appendRotation(d->roation, geom::Z_AXIS);
			}
			
			m.appendTranslation(d->tx , d->ty , 0);

			
			d->_leftu = leftu;
			d->_rightu = rightu;
			d->_topv = topv;
			d->_bottomv = bottomv;

			geom::Vector4 _np1 = geom::Vector4(0.0f, 0.0f, 0.0f);
			geom::Vector4 _np2 = geom::Vector4(1.0f, 0.0f, 0.0f);
			geom::Vector4 _np3 = geom::Vector4(0.0f, 1.0f, 0.0f);
			geom::Vector4 _np4 = geom::Vector4(1.0f, 1.0f, 0.0f);
			
			/*if (d.isSDF)   //SDF字体特殊处理...原flash代码已在读取时修改了宽高
			{
				_np1.setTo(img.PadLeft / img.ClipWidth, 0, 0);
				_np4.setTo(img.Width / img.ClipWidth, 1, 0);
			}*/

			d->p1 = _np1 * m;
			d->p2 = _np2 * m;
			d->p3 = _np3 * m;
			d->p4 = _np4 * m;

			d->_screenleft = math::minf( math::minf( math::minf(d->p1.x, d->p2.x) ,d->p3.x ) , d->p4.x );
			d->_screenright = math::maxf(math::maxf(math::maxf(d->p1.x, d->p2.x), d->p3.x), d->p4.x);
			d->_screentop = math::minf(math::minf(math::minf(d->p1.y, d->p2.y), d->p3.y), d->p4.y);
			d->_screenbottom = math::maxf(math::maxf(math::maxf(d->p1.y, d->p2.y), d->p3.y), d->p4.y);


			auto stage = CFLContext::getCurrentInstance()->stage;

			//屏幕外区域裁剪
			if (d->_screenbottom <= 0 || d->_screenright <= 0 || d->_screentop >= stage->stageHeight() || d->_screenleft >=stage->stageWidth() )
			{
				return false;
			}

			return true;

		}

		//检查两个draw信息之间是否不兼容
		static bool checkchange(drawElement* d1, drawElement* d2)
		{
			if (d1->image.isNull() || d2->image.isNull())
			{
				return true;
			}

			if (d1->blendmode != d2->blendmode ||
				d1->filtermode !=d2->filtermode
				//|| (d1.isSDF != d2.isSDF || Math.abs(d1.sdfratio - d2.sdfratio) > 0.00001)
				)
			{
				return true;
			}
			
			if (!(d1->effect->checkCompatible(d2->effect.get())))
			{
				return true;
			}
			

			std::shared_ptr<Texture2DRef>& ref1 = d1->image->refTexture;
			std::shared_ptr<Texture2DRef>& ref2 = d2->image->refTexture;

			return !( ref1->equals(ref2.get()) );
		}
	}
}
