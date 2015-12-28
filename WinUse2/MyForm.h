#pragma once

namespace WinUse2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:

		static MyForm^ ins = gcnew MyForm();

		static MyForm^ GetInstance()
		{
			return ins;
		}

		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此处添加构造函数代码
			//
		}


		void setLabel(int fps)
		{
			if (this->InvokeRequired)
			{
				this->BeginInvoke( gcnew Action<int>( this , &WinUse2::MyForm::setLabel ),fps);
			}
			else
			{
				label1->Text = String::Format( L"fps:{0}" ,fps);
			}

		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::Button^  button1;
	public: System::Windows::Forms::Button^  btnTermDisplay;
	public: System::Windows::Forms::Button^  btnBuildDisplay;
	private: System::Windows::Forms::Button^  btnCreateRGBATex;
	private: System::Windows::Forms::Button^  btnCreateETCSeperate;
	private: System::Windows::Forms::Button^  btnCreateAlatas;
	private: System::Windows::Forms::Button^  btnKTXContent;

	public:

	public:
	protected:

	protected:

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->btnTermDisplay = (gcnew System::Windows::Forms::Button());
			this->btnBuildDisplay = (gcnew System::Windows::Forms::Button());
			this->btnCreateRGBATex = (gcnew System::Windows::Forms::Button());
			this->btnCreateETCSeperate = (gcnew System::Windows::Forms::Button());
			this->btnCreateAlatas = (gcnew System::Windows::Forms::Button());
			this->btnKTXContent = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(55, 15);
			this->label1->TabIndex = 0;
			this->label1->Text = L"label1";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(347, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// btnTermDisplay
			// 
			this->btnTermDisplay->Location = System::Drawing::Point(16, 46);
			this->btnTermDisplay->Name = L"btnTermDisplay";
			this->btnTermDisplay->Size = System::Drawing::Size(119, 23);
			this->btnTermDisplay->TabIndex = 2;
			this->btnTermDisplay->Text = L"termDisplay";
			this->btnTermDisplay->UseVisualStyleBackColor = true;
			// 
			// btnBuildDisplay
			// 
			this->btnBuildDisplay->Location = System::Drawing::Point(276, 46);
			this->btnBuildDisplay->Name = L"btnBuildDisplay";
			this->btnBuildDisplay->Size = System::Drawing::Size(146, 23);
			this->btnBuildDisplay->TabIndex = 3;
			this->btnBuildDisplay->Text = L"buildDisplay";
			this->btnBuildDisplay->UseVisualStyleBackColor = true;
			// 
			// btnCreateRGBATex
			// 
			this->btnCreateRGBATex->Location = System::Drawing::Point(16, 87);
			this->btnCreateRGBATex->Name = L"btnCreateRGBATex";
			this->btnCreateRGBATex->Size = System::Drawing::Size(119, 23);
			this->btnCreateRGBATex->TabIndex = 4;
			this->btnCreateRGBATex->Text = L"createRGBAtex";
			this->btnCreateRGBATex->UseVisualStyleBackColor = true;
			this->btnCreateRGBATex->Click += gcnew System::EventHandler(this, &MyForm::btnCreateRGBATex_Click);
			// 
			// btnCreateETCSeperate
			// 
			this->btnCreateETCSeperate->Location = System::Drawing::Point(16, 117);
			this->btnCreateETCSeperate->Name = L"btnCreateETCSeperate";
			this->btnCreateETCSeperate->Size = System::Drawing::Size(119, 23);
			this->btnCreateETCSeperate->TabIndex = 5;
			this->btnCreateETCSeperate->Text = L"createETCSeperate";
			this->btnCreateETCSeperate->UseVisualStyleBackColor = true;
			this->btnCreateETCSeperate->Click += gcnew System::EventHandler(this, &MyForm::btnCreateETCSeperate_Click);
			// 
			// btnCreateAlatas
			// 
			this->btnCreateAlatas->Location = System::Drawing::Point(16, 147);
			this->btnCreateAlatas->Name = L"btnCreateAlatas";
			this->btnCreateAlatas->Size = System::Drawing::Size(119, 23);
			this->btnCreateAlatas->TabIndex = 6;
			this->btnCreateAlatas->Text = L"createETCAlatas";
			this->btnCreateAlatas->UseVisualStyleBackColor = true;
			this->btnCreateAlatas->Click += gcnew System::EventHandler(this, &MyForm::btnCreateAlatas_Click);
			// 
			// btnKTXContent
			// 
			this->btnKTXContent->Location = System::Drawing::Point(16, 177);
			this->btnKTXContent->Name = L"btnKTXContent";
			this->btnKTXContent->Size = System::Drawing::Size(119, 23);
			this->btnKTXContent->TabIndex = 7;
			this->btnKTXContent->Text = L"createKTXContent";
			this->btnKTXContent->UseVisualStyleBackColor = true;
			this->btnKTXContent->Click += gcnew System::EventHandler(this, &MyForm::btnKTXContent_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(444, 367);
			this->Controls->Add(this->btnKTXContent);
			this->Controls->Add(this->btnCreateAlatas);
			this->Controls->Add(this->btnCreateETCSeperate);
			this->Controls->Add(this->btnCreateRGBATex);
			this->Controls->Add(this->btnBuildDisplay);
			this->Controls->Add(this->btnTermDisplay);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: 
		System::Void btnCreateRGBATex_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnCreateETCSeperate_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnCreateAlatas_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btnKTXContent_Click(System::Object^  sender, System::EventArgs^  e);
};
}
