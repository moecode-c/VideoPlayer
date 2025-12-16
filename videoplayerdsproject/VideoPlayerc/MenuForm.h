#pragma once

namespace VideoPlayerc {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MenuForm : public System::Windows::Forms::Form
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
        System::ComponentModel::Container^ components;
        System::Windows::Forms::Button^ btnVideoPlayer;
        System::Windows::Forms::Button^ btnPhotoPlayer;
        System::Windows::Forms::Label^ lblTitle;
        System::Windows::Forms::PictureBox^ pictureBoxBackground;

        void InitializeComponent(void)
        {
            this->btnVideoPlayer = (gcnew System::Windows::Forms::Button());
            this->btnPhotoPlayer = (gcnew System::Windows::Forms::Button());
            this->lblTitle = (gcnew System::Windows::Forms::Label());
            this->pictureBoxBackground = (gcnew System::Windows::Forms::PictureBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->BeginInit();
            this->SuspendLayout();

            // pictureBoxBackground
            this->pictureBoxBackground->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxBackground->Location = System::Drawing::Point(0, 0);
            this->pictureBoxBackground->Name = L"pictureBoxBackground";
            this->pictureBoxBackground->Size = System::Drawing::Size(800, 600);
            this->pictureBoxBackground->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->pictureBoxBackground->TabIndex = 0;
            this->pictureBoxBackground->TabStop = false;
            this->pictureBoxBackground->BackColor = System::Drawing::Color::FromArgb(44, 62, 80);

            // lblTitle
            this->lblTitle->BackColor = System::Drawing::Color::Transparent;
            this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 36, System::Drawing::FontStyle::Bold));
            this->lblTitle->ForeColor = System::Drawing::Color::White;
            this->lblTitle->Location = System::Drawing::Point(150, 100);
            this->lblTitle->Name = L"lblTitle";
            this->lblTitle->Size = System::Drawing::Size(500, 80);
            this->lblTitle->TabIndex = 1;
            this->lblTitle->Text = L"Video Player";
            this->lblTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;

            // btnVideoPlayer
            this->btnVideoPlayer->BackColor = System::Drawing::Color::FromArgb(52, 152, 219);
            this->btnVideoPlayer->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnVideoPlayer->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold));
            this->btnVideoPlayer->ForeColor = System::Drawing::Color::White;
            this->btnVideoPlayer->Location = System::Drawing::Point(250, 250);
            this->btnVideoPlayer->Name = L"btnVideoPlayer";
            this->btnVideoPlayer->Size = System::Drawing::Size(300, 80);
            this->btnVideoPlayer->TabIndex = 2;
            this->btnVideoPlayer->Text = L"🎬 Videos";
            this->btnVideoPlayer->UseVisualStyleBackColor = false;
            this->btnVideoPlayer->Click += gcnew System::EventHandler(this, &MenuForm::btnVideoPlayer_Click);

            // btnPhotoPlayer
            this->btnPhotoPlayer->BackColor = System::Drawing::Color::FromArgb(46, 204, 113);
            this->btnPhotoPlayer->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnPhotoPlayer->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold));
            this->btnPhotoPlayer->ForeColor = System::Drawing::Color::White;
            this->btnPhotoPlayer->Location = System::Drawing::Point(250, 360);
            this->btnPhotoPlayer->Name = L"btnPhotoPlayer";
            this->btnPhotoPlayer->Size = System::Drawing::Size(300, 80);
            this->btnPhotoPlayer->TabIndex = 3;
            this->btnPhotoPlayer->Text = L"📷 Photos";
            this->btnPhotoPlayer->UseVisualStyleBackColor = false;
            this->btnPhotoPlayer->Click += gcnew System::EventHandler(this, &MenuForm::btnPhotoPlayer_Click);

            // MenuForm
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(800, 600);
            this->Controls->Add(this->btnPhotoPlayer);
            this->Controls->Add(this->btnVideoPlayer);
            this->Controls->Add(this->lblTitle);
            this->Controls->Add(this->pictureBoxBackground);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->Name = L"MenuForm";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"Video Player - Main Menu";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->EndInit();
            this->ResumeLayout(false);
        }

        System::Void btnVideoPlayer_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnPhotoPlayer_Click(System::Object^ sender, System::EventArgs^ e);
        void OnChildFormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
    };
}