// TestEntry.cpp : 
#define STRINGIFY(A)  #A

#include "CFLEntry.h"
#include "CFLFile.h"
#include "CFLString.h"
#include "CFLAsyncFunc.h"
#include "Content/CFLBinaryCache.h"


#include "Content/CFLBuildInData.h"
#include "Content/CFLFileDataResource.h"
#include "Graphic/CFLVertexPointer.h"



#include "CFLDrawCommand.h"

#include <vector>
#include <random>


#include "Geom/CFLGeom.h"
#include "Geom/CFLRectangle.h"
#include "CFLMath.h"
#include "Content/CFLCTF.h"

#include "Content/CFLContent.h"
#include "Text/CFLEncoding.h"

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "Graphic/CFLFont.h"

//adb shell top -m 10
//adb shell dumpsys meminfo com.app.TestEntry

void testfileread();
void testgeom();


using namespace cfl::tasks;
using namespace cfl;
cfl::CFLContext* context;


content::BuildInData getTestData()
{
	static GLfloat vVertices[] = { -0.5f, 0.5f, 0.0f,
		0.0f,0.0f,

		-0.5f, -0.5f, -0.0f,
		0.0f,1.0f,

		0.5f, -0.5f, -0.0f,
		1.0f,1.0f ,
	
		0.5f,0.5f, 0.0f,
		1.0f,0.0f

	};
	
	return content::BuildInData(vVertices, sizeof(vVertices));
}


float times = 0;
bool hasinitshader = false;

render::GLProgram* prog;
render::VertexPointer* vtx;
render::textures::Texture2D* tex2d;

void up(CFLContext* context,float dettime)
{
	context->graphic->clear(0, 0, 0.0f, 1, 0, 0, cfl::graphic::ClearMaskBit::color);
	
	static int mmm = 0;

	times += dettime;
	//cfl::trace("%f\n", dettime);
	if (times >= 1)
	{
		LOGI("%d\n", (int)cfl::getFPS());
		times = 0;

		if (prog && prog->isValid())
		{
			LOGI("location: %d\n", prog->getAttribute("vPosition")->location);


		}

	}
	
	if (prog && prog->isValid() )
	{

		auto iimg = content::Content::loadGameImage("testreadfile/commimage1/新资源/公用资源/竞技场-皇榜-上.png");
		auto iimg2 = content::Content::loadGameImage("testreadfile/commimage1/旗子导出2/白右/baiqiyou_00005.png");
		auto iimg3 = content::Content::loadGameImage("testreadfile/commimage1/界面文字/界面文字-标题/图鉴.png");
				

		context->graphic->drawGameImage(content::Content::loadGameImage("testreadfile/commimage1/主界面背景图.png"), 0.0f, 0.0f, 1.0f, nullptr, 1.0f);
		//context->graphic->drawGameImage(content::Content::loadGameImage("testreadfile/commimage1/主界面背景图.png"), 0.0f, 0.0f, 1.0f, nullptr, 1.0f);

		//context->graphic->testDrawVertex(prog, *vtx,*tex2d);
		for (size_t i = 0; i < 1; i++)
		{
			context->graphic->drawGameImage(iimg2, 0.0f, 0.0f ,1.0f, nullptr, 1.0f);

			context->graphic->drawGameImage(
				iimg, 140.0f, 400.0f , &geom::Matrix3D().appendRotation(toRadians(context->totalframes *1.0f / 60) * 15, geom::Y_AXIS), 1.0f, nullptr, 1.0f);// , context->totalframes *1.0f / 60 * PI );

				

			context->graphic->drawGameImage(iimg3, 0.0f, 0.0f, 1.0f, nullptr, 1.0f);
		}
			

		/*for (size_t i = 0; i < 100; i++)
		{
			context->graphic->drawLine(geom::Vector2(200, 200),
				geom::Vector2(200, 200) + geom::Vector2(300 * math::cosf(toRadians(i * 360.0f / 200)),
				200 *math::sinf(toRadians(i * 360.0f / 200))),

				graphic::Color(math::cosf((context->totalframes % 30)  * PI / 30 * 2)*0.5f + 0.5f,
				math::sinf((context->totalframes % 45)  * PI / 45 * 2)*0.5f + 0.5f,
				math::cosf((context->totalframes % 60)  * PI / 60 * 2)*0.5f + 0.5f, 1),

				((context->totalframes + i) % 200) / 10);
		}*/

			
		//context->graphic->testDrawVertex(prog, *vtx, *tex2d);

		for (size_t i = 0; i < 100; i++)
		{
			context->graphic->drawGameImage(iimg, 300.0f+i*10, 300.0f,
				((context->totalframes + i*3)  % 60)/60.0f ,nullptr,0.15f
				, toRadians( i*5 + context->totalframes )
				);

			context->graphic->drawGameImage(iimg2, 505.0f + i * 3, 167.0f, 1.0f, nullptr, i/10.0f * 0.5f, toRadians( i*context->totalframes/2) );


			context->graphic->drawGameImage(iimg3, 570.0f +i*20 , 426.0f + i * 10, 
				&geom::Matrix3D().appendRotation( toRadians(i*i+context->totalframes),geom::Y_AXIS ) ,1.0f, nullptr, i/100.0f+0.2f);
		}
		
		
		auto font = graphic::font::Font::getFont("arial");

		auto gimg = font.getGlyph('S')->glyphImage;

		auto g = font.getGlyph((context->totalframes / 5) % 1000);
		if (g)
		{ 
			auto ggimg = g->glyphImage;
			//context->graphic->drawGameImage(ggimg, 0, 0, 1.0f, nullptr, 0.5f);
			context->graphic->drawString(ggimg,10,10,16);
		}
		else
		{
			//context->graphic->drawGameImage(gimg, 0, 0, 1.0f, nullptr, 0.5f);
			context->graphic->drawString(gimg,10,10,16);
		}
		gimg = font.getGlyph('K')->glyphImage;
		context->graphic->drawString(gimg, 20, 10, 26);
		gimg = font.getGlyph('b')->glyphImage;
		context->graphic->drawString(gimg, 55, 10, 30);
		gimg = font.getGlyph('b')->glyphImage;
		context->graphic->drawString(gimg, 85, 10, 16);
		gimg = font.getGlyph('c')->glyphImage;
		context->graphic->drawString(gimg, 101, 10, 128);

		//font = graphic::font::Font::getFont("simsun");
		//gimg = font.getGlyphForRending(CFLString("课").charCodeAt(0))->glyphImage;
		//context->graphic->drawString(gimg, 301, 10, math::sinf(((context->totalframes ) % 60)/60.0f*3.14)*256+24 );

		/*if (tex2d != nullptr )
		{
			delete tex2d;
			tex2d = nullptr;
		}*/
		
	}
	
	testgeom();
	//testfileread();
	
	if (!hasinitshader)
	{
		hasinitshader = true;

		auto shader = context->glObjCollection->createShader();


		const char* vShaderStr = STRINGIFY(
			attribute vec4 vPosition;
			attribute vec2 a_texCoord;
			varying vec2 v_texCoord;
		void main()
		{
			gl_Position = vPosition;
			v_texCoord = a_texCoord;
		}

		);


		auto fshader = context->glObjCollection->createShader();
		const char* fShaderStr = STRINGIFY(
			precision mediump float;
			varying vec2 v_texCoord;
			uniform sampler2D s_texture;
			void main()
			{
				gl_FragColor =   texture2D(s_texture, v_texCoord);
			}
		);


		shader->compile(vShaderStr, cfl::render::ShaderType::vertex_shader);
		fshader->compile(fShaderStr, cfl::render::ShaderType::fragment_shader);

		auto program = context->glObjCollection->createProgram();
		program->link(shader, fshader);

		prog = program;
		//auto data = reinterpret_cast<GLfloat*>( getTestData().getData());
		auto data = getTestData();

		data.loadAsync([](std::shared_ptr< content::IGLDataResource> ds,void * args ){
			auto k = ds->isDone();
		},nullptr,nullptr);

		render::VertexPointer* vp =
			new render::VertexPointer(0, 3, cfl::render::vertexPointerType::glFloat , false, 5*sizeof(GLfloat),
				std::make_shared<content::BuildInData>(data));


		vtx = vp;



		auto getPixels = []()-> std::shared_ptr< content::BuildInData>
		{
			//static GLubyte pixels[4 * 3] =
			//{
			//	255, 0,0, // Red
			//	0, 255,0, // Green
			//	0, 0,255, // Blue
			//	255, 255,0// Yellow
			//};
			//return std::make_shared<content::BuildInData>(pixels, sizeof(pixels));

			auto rgbadata = cfl::content::BinaryCache::getInstance()->getData(cfl::file::DirType::asset, "testreadfile/outrgba.dat");
			auto data = rgbadata.getData();

			content::CTFHeader header;
			memcpy(&header, data, sizeof(header));

			data += sizeof(header);

			static char* cache = nullptr;
			if (!cache)
			{
				cache = (char *) malloc(header.length);
				memcpy(cache, data, header.length);

			}


			return std::make_shared<content::BuildInData>(cache, header.length );
		};

		tex2d = new render::textures::Texture2D();

		tex2d->addUploadEventHandler(
			[]( render::textures::Texture2D* tex,void* args )
		{
			LOGI("tex progress: %f\n", tex->getPorgress());
		}, 
			
			nullptr, nullptr);
		tex2d->initFromFile(cfl::file::DirType::asset, "testreadfile/ktx1.ktx", true, true);


		//auto fs = std::make_shared<content::FileDataResource>(cfl::file::DirType::asset, "testreadfile/outrgba.dat");

		/*tex2d = new render::textures::Texture2D(render::tex2d_internalformat::glrgba,1024,512);
		tex2d->uploadFromImageData(0, 1024, 512, render::tex2d_inputformat::glrgba,
			render::tex2d_pixeltype::glubyte, fs,
			1024 * 4,
			sizeof(content::CTFHeader),
			true
			);*/

		/*tex2d->appendMipLevel(0, render::tex2d_internalformat::glrgba, 1024, 512,
			render::tex2d_inputformat::glrgba, render::tex2d_pixeltype::glubyte, fs, sizeof(content::CTFHeader) , 1024*4);
		*/

	}

};

//#define ACCESS _access  
//#define MKDIR(a) _mkdir((a))



void* testdo();
using namespace cfl;
int cfl_main(cfl::CFLContext* ctx, int argc, char* argv[])
{
	context = ctx;

	ctx->stage->scaleMode(StageScaleMode::SHOW_ALL);
	ctx->stage->stageWidth(1280);
	ctx->stage->stageHeight(720);


	//cfl::setFPS(30);
	hasinitshader = false;
	prog = nullptr;

	cfl::registerUpdateFunc(ctx, up);

	//testdo();

	cfl::content::BinaryCache::getInstance()->setMaxCacheSize(1024 * 1024 * 2);

	
	content::Content::initGameImageFromFile(file::DirType::asset, "testreadfile/commimage1/imgfilecfg.bin", "testreadfile/commimage1/");

	CFLString img = "testreadfile/commimage1/装饰纹案3/装饰纹案-3_05.png";

	/*auto gbkenc= cfl::text::Endoder::getGBK();
	auto string= gbkenc->getString("testreadfile/commimage1/装饰纹案3/装饰纹案-3_05.png");
	*/
	auto exists =content::Content::isExists(img);

	
	LOGI(" GameImage exists:%d, path: %s \n", exists, img.log_str());
	auto iimg=content::Content::loadGameImage("testreadfile/commimage1/icon_预览.png");
	iimg->refTexture->upload(true);
	
	UChar c('A');
	auto cat = UChar::getUnicodeCategory(c);

	auto cc = UChar::toLower(c);

	if (c ==  UChar::toUpper( cc))
	{
		LOGI(" %c==%c\n", c.charCode, cc.charCode);
	}

	LOGI(" %s,len: %d\n", img.log_str(), img.length());

	CFLString unichar = CFLString(c);
	
	auto len = unichar.length();
	auto acc= unichar.log_str();

	auto ucc2 = unichar + unichar;

	unsigned int  ucs4code[4] = { 0x0627, 0x0655, 0x0650, 0 };
	CFLString uni2(ucs4code);

	auto tick = cfl::getTimer();

	CFLString add("abcd哈哈");
	for (size_t i = 0; i < 100; i++)
	{
		uni2 += add;
	}
	len = uni2.length();

	auto t = cfl::getTimer() - tick;

	LOGI("timer: %d,%s\n",t,(uni2+c).log_str());



	unsigned int  u4code[7] = { 0x0627, 0x0655, 0x0650, 0x064A, 0x0647, 0x064E, 0 };

	unsigned int u4code2[4] = { 0x0627,0x0628,0x064C,0 };

	CFLString myString(u4code);

	text::TextElementEnumerator tee(myString + CFLString(u4code2) );
	tee.reset();
	while (tee.moveNext())
	{
		auto te = tee.getCurrent();
		LOGI("st:%d,len:%d ", te->index,te->len );
		for (size_t i = te->index; i < te->index + te->len; i++)
		{
			LOGI("hex: %04X ", te->uchars[i]);
		}
		LOGI("\n");
	}
	tick = cfl::getTimer();
	auto afont = cfl::graphic::font::Font::init(cfl::file::DirType::asset, "fonts/ARIAL.ff");
	t = cfl::getTimer() - tick;
	LOGI("font init timer: %d\n", t);

	tick = cfl::getTimer();
	for (size_t i = 0x21; i < 1024; i++)
	{
		auto k = afont.queryKerning(32, 932);

		afont.getGlyphForRending(i);
	}
	
	t = cfl::getTimer() - tick;
	LOGI("query timer: %d\n", t);

	afont.getGlyphForRending('R');
	afont.getGlyphForRending('S');
	cfl::graphic::font::Font::regFont(afont, "arial");

	//tick = cfl::getTimer();
	//auto hfont = cfl::graphic::font::Font::init(cfl::file::DirType::asset, "fonts/SIMSUN.ff"); 
	//
	//cfl::graphic::font::Font::regFont(hfont, "simsun");
	//t = cfl::getTimer() - tick;
	//LOGI("simsun init timer: %d\n", t);
	
	//render::textures::Texture2D texture;
	//texture.initFromFile(file::DirType::asset, "testreadfile/ktx888.ktx",false);

	
	//auto t = node.data();


	/*auto ctfheadersize = sizeof(content::CTFHeader);

	auto rgbadata=cfl::content::BinaryCache::getInstance()->getData(cfl::file::DirType::asset, "testreadfile/outrgba.dat");
	auto data = rgbadata.getData();
	
	content::CTFHeader header;
	memcpy(&header, data, ctfheadersize);

	data += ctfheadersize;
	
	CFL_ASSERT(header.length == rgbadata.getResourceSize() - ctfheadersize);*/

	


	//unsigned char str[] = {0xe4, 0xb8, 0xad, 0xe6, 0x96, 0x87, 0xe8, 0xb7, 0xaf, 0xe5, 0xbe, 0x84, 0x2f, 0x54, 0x68, 0x75,
	//	0x6e, 0x64, 0x65, 0x72, 0x2e, 0x74, 0x78, 0x74,0}; //"中文路径/Thunder.txt"; 成功获取中文路径文件。说明路径需转为utf8
	//
	//auto file = cfl::file::getDirectory(cfl::file::DirType::document)->getfile( reinterpret_cast< char*>( str));

	//cfl::trace("file: %s,isExists %d \n",file->getFilePath() ,file->isExists());

	/*char* k = "olé";
	std::hash< char*> hash;
	auto s = hash(k);*/
	
	
	//CFL_ASSERT(p1 == p2);
	//CFL_ASSERT(p1 == p3);
	
	
	return 0;
}

void testgeom()
{
	geom::Matrix mat(3.0f, 4.56f, -5, 6, 9, -4);

	//auto mat2 = mat;

	//mat.identity();
	mat.rotate(toRadians(15));

	mat.scale(5, 13);

	mat.translate(30, -40);

	mat.rotate(toRadians(-73));

	//mat2.append(mat);

	auto inv = mat.getInvert();

	auto test = mat * inv;

	auto v = mat.transformPoint(geom::Vector2(40.44f, 55.061f));
	//(x=5239.566531818498, y=2993.9335602476685)

	auto v2 = inv.transformPoint(v);

	auto k = geom::Vector2(40.44f, 55.061f) * mat;
	//LOGI("x=%f,y=%f\n", k.x, k.y);

	//CFL_ASSERT(geom::Vector2(40.44f, 55.061f) == v2);

	geom::Matrix3D mat3d(mat);
	
	auto det = mat3d.determinant();//应该等于determinant = 2651.999755859375
	
	auto mat3dinv = mat3d.getInvert();
	
	auto testidentity = mat3d * mat3dinv;


	//mat3d.identity();
	//mat.identity();

	//mat.rotate(toRadians(25));

	mat3d.appendRotation(toRadians(25), geom::Y_AXIS);
	mat3d.appendScale(3, 2, 1);
	mat3d.appendRotation(toRadians(-12), geom::X_AXIS);

	auto tovec = geom::Vector4(3, 2, 1) * mat3d;
	//Vector3D(707.3577880859375, 49.67710876464844, -121.83626556396484)
	mat3d.invert();
	auto invp = tovec * mat3d;


	geom::Quaternion qu1; qu1.fromAxisAngle(geom::Vector3(1.5, 0.5, 33), toRadians(55));

	geom::Matrix3D toro;
	
	qu1.setMatrix3D(&toro);

	geom::Quaternion qu2; qu2.fromMatrix3D(toro);

	

	geom::Rectangle firstRect= geom::Rectangle();
	//LOGI("%s\n",firstRect.toString().log_str());  // (x=0, y=0, w=0, h=0)
	geom::Rectangle secondRect = geom::Rectangle(1, 3, 11, 13);
	//LOGI("%s\n", secondRect.toString().log_str()); // (x=1, y=3, w=11, h=13)
	geom::Rectangle thirdRect = geom::Rectangle(5, 8, 17, 19);
	//LOGI("%s\n", thirdRect.toString().log_str());  // (x=5, y=8, w=17, h=19)
	bool isContained  = secondRect.containsRect(thirdRect);
	//LOGI(isContained); // false


}


void testfileread()
{


	auto bincache = cfl::content::BinaryCache::getInstance();

	std::vector<CFLString> toreadfiles(
	{
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "big" }
	);
	std::vector<CFLString> files2({
		"diffuse0.gif.tgz", "lod_1.ass.tgz", "lod_2.ass.tgz", "model.ass.tgz"
	});

	std::vector<CFLString> bigs({
		"Battlefield01.mp3.tgz", "LIMPIDLUSTER.mp3.tgz", "Login.mp3.tgz"
		, "Map.mp3.tgz", "Scene.mp3.tgz"
	});

	std::srand(getTimer());


	for (size_t i = 0; i < 15; i++)
	{
		auto rnd = std::rand() % 3;

		auto p1 = toreadfiles[std::rand() % (toreadfiles.size())];
		auto p2 = files2[std::rand() % (files2.size())];

		if (p1 == CFLString("big"))
		{
			p2 = bigs[std::rand() % (bigs.size())];
		}

		auto fullpath = "testreadfile/" + p1 + "/" + p2;

		

		if (rnd == 2)
		{

			auto k = content::BinaryCache::getInstance()->getData(file::DirType::asset, "testreadfile/file.txt");
			CFL_ASSERT(CFLString(k.getData()) == CFLString("内容"));
			auto kd = k.getData();
			//trace("%s \n", kd);



		}
		else
			if (rnd == 0 && false)
			{
				auto gf = [](CFLString fullpath, cfl::content::BinaryCache* bincache)
				{
					try
					{
						auto data = bincache->getData(file::DirType::asset, fullpath);
						CFL_ASSERT(data.getStatus() == file::FileIoStatus::success);
						if (data.getStatus() != file::FileIoStatus::success)
						{
							auto cc = data.getResoucePath().c_str();

							LOGE("sync filepath:%s error!! \n", fullpath.log_str());
						}
						else
						{
							auto cc = data.getResoucePath();
							//LOGI("sync filepath:%s success \n", cc.c_str());
						}
					}
					catch (content::BinaryDataDeathLockException ex)
					{
						LOGE("%s\n", ex.what());
					}

				};
				//for (size_t i = 0; i < 20; i++)
				{

					asyncFunc::beginInvoke(gf, fullpath, bincache);


				}

				//trace("call begininvoke %s\n",fullpath.c_str());
			}
			else
			{
				//trace("async %s\n", fullpath.c_str());
				auto h = bincache->getDataAsync(file::DirType::asset, fullpath
					,
					[](content::BinaryData* data,void* args)
				{
					auto path = data->getResoucePath().c_str();
					auto prog = (float)(data->getReadSize()) / data->getResourceSize();

					//LOGI("filepath:%s,progress: %f\n", data->getResoucePath().c_str(), (float)(data->getReadSize()) / data->getResourceSize());
				}
					,
					[](content::BinaryData* data,void* args)
				{
					if (data->getStatus() != file::FileIoStatus::success)
					{
						CFL_ASSERT(false);

						//LOGE("filepath:%s error!! \n", data->getResoucePath().c_str());

					}
					else
					{
						auto path = data->getResoucePath().c_str();

						//LOGI("filepath:%s over \n", data->getResoucePath().c_str());
					}

					int* value= reinterpret_cast<int*>(args);
					//LOGI("value: %d\n", *value);

					delete value;
				},
					new int(i)
					);
				
			}
	}

	auto s = cfl::content::BinaryCache::getInstance()->getCachedSize();

}


