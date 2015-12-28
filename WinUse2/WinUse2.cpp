// WinUse2.cpp: 主项目文件。

#include "stdafx.h"


#include "CFLEntry.h"
#include "CFLFile.h"
#include "CFLString.h"
#include "Content/CFLBinaryCache.h"

#include "Content/CFLBuildInData.h"
#include "Graphic/CFLVertexPointer.h"

#include "Graphic/CFLTexture.h"
#include "Content/CFLFileDataResource.h"
#include "Content/CFLCTF.h"

#include <vector>
#include <random>

#include "MyForm.h"

#define STRINGIFY(A)  #A

using namespace System;

//int main(array<System::String ^> ^args)
//{
//    Console::WriteLine(L"Hello World");
//
//    return 0;
//}

using namespace cfl;

void OnClosed(System::Object ^sender, System::EventArgs ^e);
cfl::CFLContext* context;

void OnClick(System::Object ^sender, System::EventArgs ^e);
void OnTermDisplayClick(System::Object ^sender, System::EventArgs ^e);
void OnBuildDisplayClick(System::Object ^sender, System::EventArgs ^e);


bool hasinitshader = false;

render::GLProgram* prog;
render::VertexPointer* vtx;
render::textures::Texture2D* tex2d;

void init()
{
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
			gl_FragColor = texture2D(s_texture, v_texCoord);
		}
		);


		shader->compile(vShaderStr, cfl::render::ShaderType::vertex_shader);
		fshader->compile(fShaderStr, cfl::render::ShaderType::fragment_shader);

		auto program = context->glObjCollection->createProgram();
		program->link(shader, fshader);

		prog = program;
		
		auto getVertexPointer = []()->std::shared_ptr < content::BuildInData >
		{
			static GLfloat vVertices[] = { -0.5f, 0.5f, 0.0f,
				0.0f, 0.0f,

				-0.5f, -0.5f, -0.0f,
				0.0f, 1.0f,

				0.5f, -0.5f, -0.0f,
				1.0f, 1.0f,

				0.5f, 0.5f, 0.0f,
				1.0f, 0.0f

			};

			return std::make_shared<content::BuildInData>(vVertices, sizeof(vVertices));
		};


		auto data = getVertexPointer();

		render::VertexPointer* vp =
			new render::VertexPointer(0, 3, cfl::render::vertexPointerType::glFloat, false, 5 * sizeof(GLfloat),
			data);


		vtx = vp;



		auto getPixels = []()-> std::shared_ptr< content::BuildInData>
		{
			static GLubyte pixels[4 * 3] =
			{
				255, 0, 0, // Red
				0, 255, 0, // Green
				0, 0, 255, // Blue
				255, 255, 0// Yellow
			};
			return std::make_shared<content::BuildInData>(pixels, sizeof(pixels));

		};

		/*tex2d = new render::textures::Texture2D(render::tex2d_internalformat::glrgb, 2, 2);
		tex2d->uploadFromImageData(0, 2, 2, 
			render::tex2d_inputformat::glrgb, 
			render::tex2d_pixeltype::glubyte, 
			getPixels(), 3, 0,true);*/

		//tex2d = context->glObjCollection->createTex2D();
		//tex2d->appendMipLevel(0, render::tex2d_internalformat::glrgb, 2, 2,
		//	render::tex2d_inputformat::glrgb, render::tex2d_pixeltype::glubyte, getPixels(),0, 3);

		//tex2d->texImage2d();

	}
}



void up(CFLContext* context, float dettime)
{
	static int mmm = 0;

	init();

	context->graphic->clear(0, 0, 0.9, 1, 0, 0, cfl::graphic::ClearMaskBit::color | cfl::graphic::ClearMaskBit::depth);

	if (prog && prog->isValid())
	{
		if (tex2d == nullptr)
		{
			const char* res;
			if (std::rand() % 2 == 0)
			{
				res = "testreadfile/outrgba2.dat";
			}
			else
			{
				res = "testreadfile/outrgba.dat";
			}

			auto fs =
				std::make_shared<content::FileDataResource>(cfl::file::DirType::asset,
				res);

			tex2d = new render::textures::Texture2D(render::tex2d_internalformat::glrgba, 1024, 512);
			tex2d->uploadFromImageData(0, 1024, 512, render::tex2d_inputformat::glrgba,
				render::tex2d_pixeltype::glubyte, fs,
				1024 * 4,
				sizeof(content::CTFHeader),
				true
				);
		}


		for (size_t i = 0; i < 1000; i++)
		{
			context->graphic->drawLine(geom::Vector2(0, 0),
				geom::Vector2(0.5 * math::cosf(toRadians(i * 360.0f / 1000.0f)),
				0.3 *math::sinf(toRadians(i * 360.0f / 1000.0f))),graphic::Color());
		}

		/*if (tex2d !=nullptr)
		{
			delete tex2d;
			tex2d = nullptr;
		}*/



		//context->graphic.testDrawVertex(prog, *vtx, *tex2d);
	}
	
	WinUse2::MyForm::GetInstance()->setLabel(getFPS()
		);
	
};



int cfl_main(cfl::CFLContext* ctx, int argc, char* argv[])
{
	context = ctx;

	cfl::registerUpdateFunc(ctx, up);

	cfl::content::BinaryCache::getInstance()->setMaxCacheSize(1024 * 1024 * 2);
	
	
	WinUse2::MyForm^ frm = WinUse2::MyForm::GetInstance();

	frm->button1->Click += gcnew System::EventHandler(&OnClick);

	frm->Show();

	frm->Closed += gcnew System::EventHandler(&OnClosed);
	

	frm->btnTermDisplay->Click += gcnew System::EventHandler(&OnTermDisplayClick);
	frm->btnBuildDisplay->Click += gcnew System::EventHandler(&OnBuildDisplayClick);

	return 0;
}



content::BuildInData getTestData()
{
	static GLfloat vVertices[] = { 0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f };

	return content::BuildInData(vVertices,sizeof(vVertices));
}

void OnClick(System::Object ^sender, System::EventArgs ^e)
{
	auto shader = context->glObjCollection->createShader();


	char* vShaderStr = STRINGIFY(
		attribute vec4 vPosition;
		void main()
		{
			gl_Position = vPosition;
		}

		);
		

	auto fshader = context->glObjCollection->createShader();
	char* fShaderStr = STRINGIFY(
		precision mediump float;
		void main()
		{ 
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
		);


	shader->compile(vShaderStr, cfl::render::ShaderType::vertex_shader);
	fshader->compile(fShaderStr, cfl::render::ShaderType::fragment_shader);

	auto program = context->glObjCollection->createProgram();
	program->link(shader, fshader);


	//auto data = reinterpret_cast<GLfloat*>( getTestData().getData());
	auto data = getTestData();
	render::VertexPointer* vp =
		new render::VertexPointer(0, 3, cfl::render::vertexPointerType::glFloat, false, 0,
		std::make_shared<content::BuildInData>(data));
	
	
}

void OnClosed(System::Object ^sender, System::EventArgs ^e)
{
	exit(1);
}

void OnTermDisplayClick(System::Object ^sender, System::EventArgs ^e)
{
	closeGL(context);
}

void OnBuildDisplayClick(System::Object ^sender, System::EventArgs ^e)
{
	buildGL(context);
}