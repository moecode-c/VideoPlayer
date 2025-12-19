#pragma once
#include "MyForm.h"
#include "PhotoPlayerForm.h"

namespace VideoPlayerc {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    ref class MainForm;

    public ref class MenuForm : public Form
    {
    public:
        MenuForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~MenuForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::IContainer^ components;
        System::Windows::Forms::Button^ btnVideo;
        System::Windows::Forms::Button^ btnPhoto;
        System::Windows::Forms::Label^ lblTitle;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->btnVideo = (gcnew System::Windows::Forms::Button());
            this->btnPhoto = (gcnew System::Windows::Forms::Button());
            this->lblTitle = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();

            // MenuForm
            this->ClientSize = System::Drawing::Size(800, 500);
            this->Name = L"MenuForm";
            this->Text = L"Photo/Video Player";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = System::Drawing::Color::FromArgb(30, 30, 40);

            // lblTitle
            this->lblTitle->AutoSize = false;
            this->lblTitle->Size = System::Drawing::Size(760, 80);
            this->lblTitle->Location = System::Drawing::Point(20, 40);
            this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 24, System::Drawing::FontStyle::Bold));
            this->lblTitle->ForeColor = System::Drawing::Color::White;
            this->lblTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            this->lblTitle->Text = L"Welcome to Video Player";

            // btnVideo
            this->btnVideo->Size = System::Drawing::Size(220, 60);
            this->btnVideo->Location = System::Drawing::Point((this->ClientSize.Width / 2) - this->btnVideo->Size.Width - 10, 160);
            this->btnVideo->Text = L"Open Video Player";
            this->btnVideo->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->btnVideo->BackColor = System::Drawing::Color::FromArgb(52, 152, 219);
            this->btnVideo->ForeColor = System::Drawing::Color::White;
            this->btnVideo->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnVideo->Click += gcnew System::EventHandler(this, &MenuForm::btnVideo_Click);

            // btnPhoto
            this->btnPhoto->Size = System::Drawing::Size(220, 60);
            this->btnPhoto->Location = System::Drawing::Point((this->ClientSize.Width / 2) + 10, 160);
            this->btnPhoto->Text = L"Open Photo Player";
            this->btnPhoto->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->btnPhoto->BackColor = System::Drawing::Color::FromArgb(46, 204, 113);
            this->btnPhoto->ForeColor = System::Drawing::Color::White;
            this->btnPhoto->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnPhoto->Click += gcnew System::EventHandler(this, &MenuForm::btnPhoto_Click);

            this->Controls->Add(this->lblTitle);
            this->Controls->Add(this->btnVideo);
            this->Controls->Add(this->btnPhoto);

            this->ResumeLayout(false);
        }

        System::Void btnVideo_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try {
                VideoPlayerc::MainForm^ main = gcnew VideoPlayerc::MainForm();
                this->Hide();
                main->ShowDialog();
                this->Show();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Unable to open video player: " + ex->Message, "Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnPhoto_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try {
                VideoPlayerc::PhotoPlayerForm^ photo = gcnew VideoPlayerc::PhotoPlayerForm();
                this->Hide();
                photo->ShowDialog();
                this->Show();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Unable to open photo player: " + ex->Message, "Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    };
}
