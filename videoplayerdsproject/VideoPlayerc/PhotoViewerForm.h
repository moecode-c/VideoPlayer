#pragma once
#include "PhotoList.h"

namespace PhotoViewer {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class PhotoViewerForm : public Form
    {
    private:
        PhotoList^ photoList;
        PictureBox^ photoDisplay;
        PictureBox^ backgroundPicture;
        ListBox^ photoListBox;
        Button^ btnUpload;
        Button^ btnDelete;
        Button^ btnNext;
        Button^ btnPrevious;
        Label^ lblPhotoName;
        Label^ lblCount;
        System::ComponentModel::Container^ components;

    public:
        PhotoViewerForm(void)
        {
            InitializeComponent();
            photoList = gcnew PhotoList();
            LoadBackground();
        }

    protected:
        ~PhotoViewerForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Text = L"Photo Gallery";
            this->Size = System::Drawing::Size(1024, 768);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(30, 41, 82);

            // Background PictureBox
            this->backgroundPicture = gcnew PictureBox();
            this->backgroundPicture->Dock = DockStyle::Fill;
            this->backgroundPicture->SizeMode = PictureBoxSizeMode::StretchImage;
            this->Controls->Add(this->backgroundPicture);

            // Photo Display PictureBox (centered on background)
            this->photoDisplay = gcnew PictureBox();
            this->photoDisplay->Location = System::Drawing::Point(300, 150);
            this->photoDisplay->Size = System::Drawing::Size(450, 400);
            this->photoDisplay->SizeMode = PictureBoxSizeMode::Zoom;
            this->photoDisplay->BackColor = Color::Transparent;
            this->photoDisplay->BorderStyle = BorderStyle::None;
            this->backgroundPicture->Controls->Add(this->photoDisplay);

            // Photo name label
            this->lblPhotoName = gcnew Label();
            this->lblPhotoName->Location = System::Drawing::Point(300, 560);
            this->lblPhotoName->Size = System::Drawing::Size(450, 30);
            this->lblPhotoName->Font = gcnew System::Drawing::Font(L"Arial", 12, FontStyle::Bold);
            this->lblPhotoName->TextAlign = ContentAlignment::MiddleCenter;
            this->lblPhotoName->BackColor = Color::Transparent;
            this->lblPhotoName->ForeColor = Color::White;
            this->backgroundPicture->Controls->Add(this->lblPhotoName);

            // Count label
            this->lblCount = gcnew Label();
            this->lblCount->Location = System::Drawing::Point(300, 590);
            this->lblCount->Size = System::Drawing::Size(450, 25);
            this->lblCount->Font = gcnew System::Drawing::Font(L"Arial", 10);
            this->lblCount->TextAlign = ContentAlignment::MiddleCenter;
            this->lblCount->Text = L"No photos uploaded";
            this->lblCount->BackColor = Color::Transparent;
            this->lblCount->ForeColor = Color::FromArgb(100, 200, 255);
            this->backgroundPicture->Controls->Add(this->lblCount);

            // ListBox for photo list (smaller, on the right)
            this->photoListBox = gcnew ListBox();
            this->photoListBox->Location = System::Drawing::Point(770, 150);
            this->photoListBox->Size = System::Drawing::Size(230, 350);
            this->photoListBox->Font = gcnew System::Drawing::Font(L"Arial", 9);
            this->photoListBox->BackColor = Color::FromArgb(40, 50, 90);
            this->photoListBox->ForeColor = Color::White;
            this->photoListBox->BorderStyle = BorderStyle::FixedSingle;
            this->photoListBox->DoubleClick += gcnew EventHandler(this, &PhotoViewerForm::photoListBox_DoubleClick);
            this->backgroundPicture->Controls->Add(this->photoListBox);

            // Upload button
            this->btnUpload = gcnew Button();
            this->btnUpload->Location = System::Drawing::Point(770, 510);
            this->btnUpload->Size = System::Drawing::Size(230, 45);
            this->btnUpload->Text = L"📤 UPLOAD";
            this->btnUpload->Font = gcnew System::Drawing::Font(L"Arial", 11, FontStyle::Bold);
            this->btnUpload->BackColor = Color::FromArgb(0, 200, 150);
            this->btnUpload->ForeColor = Color::White;
            this->btnUpload->FlatStyle = FlatStyle::Flat;
            this->btnUpload->FlatAppearance->BorderSize = 0;
            this->btnUpload->Cursor = Cursors::Hand;
            this->btnUpload->Click += gcnew EventHandler(this, &PhotoViewerForm::btnUpload_Click);
            this->backgroundPicture->Controls->Add(this->btnUpload);

            // Delete button
            this->btnDelete = gcnew Button();
            this->btnDelete->Location = System::Drawing::Point(770, 565);
            this->btnDelete->Size = System::Drawing::Size(230, 40);
            this->btnDelete->Text = L"🗑️ Delete";
            this->btnDelete->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
            this->btnDelete->BackColor = Color::FromArgb(220, 50, 80);
            this->btnDelete->ForeColor = Color::White;
            this->btnDelete->FlatStyle = FlatStyle::Flat;
            this->btnDelete->FlatAppearance->BorderSize = 0;
            this->btnDelete->Cursor = Cursors::Hand;
            this->btnDelete->Click += gcnew EventHandler(this, &PhotoViewerForm::btnDelete_Click);
            this->backgroundPicture->Controls->Add(this->btnDelete);

            // Previous button
            this->btnPrevious = gcnew Button();
            this->btnPrevious->Location = System::Drawing::Point(100, 350);
            this->btnPrevious->Size = System::Drawing::Size(120, 45);
            this->btnPrevious->Text = L"⬅️ Prev";
            this->btnPrevious->Font = gcnew System::Drawing::Font(L"Arial", 11, FontStyle::Bold);
            this->btnPrevious->BackColor = Color::FromArgb(52, 152, 219);
            this->btnPrevious->ForeColor = Color::White;
            this->btnPrevious->FlatStyle = FlatStyle::Flat;
            this->btnPrevious->FlatAppearance->BorderSize = 0;
            this->btnPrevious->Cursor = Cursors::Hand;
            this->btnPrevious->Click += gcnew EventHandler(this, &PhotoViewerForm::btnPrevious_Click);
            this->backgroundPicture->Controls->Add(this->btnPrevious);

            // Next button
            this->btnNext = gcnew Button();
            this->btnNext->Location = System::Drawing::Point(805, 350);
            this->btnNext->Size = System::Drawing::Size(120, 45);
            this->btnNext->Text = L"Next ➡️";
            this->btnNext->Font = gcnew System::Drawing::Font(L"Arial", 11, FontStyle::Bold);
            this->btnNext->BackColor = Color::FromArgb(52, 152, 219);
            this->btnNext->ForeColor = Color::White;
            this->btnNext->FlatStyle = FlatStyle::Flat;
            this->btnNext->FlatAppearance->BorderSize = 0;
            this->btnNext->Cursor = Cursors::Hand;
            this->btnNext->Click += gcnew EventHandler(this, &PhotoViewerForm::btnNext_Click);
            this->backgroundPicture->Controls->Add(this->btnNext);
        }

        void LoadBackground()
        {
            try
            {
                String^ basePath = Application::StartupPath;
                String^ bgPath = Path::Combine(basePath, "Images\\photoBackground.png");

                if (File::Exists(bgPath))
                {
                    backgroundPicture->Image = Image::FromFile(bgPath);
                }
                else
                {
                    // Fallback: create gradient background
                    Bitmap^ bmp = gcnew Bitmap(this->Width, this->Height);
                    Graphics^ g = Graphics::FromImage(bmp);
                    System::Drawing::Drawing2D::LinearGradientBrush^ brush =
                        gcnew System::Drawing::Drawing2D::LinearGradientBrush(
                            System::Drawing::Rectangle(0, 0, this->Width, this->Height),
                            Color::FromArgb(30, 41, 82),
                            Color::FromArgb(15, 20, 40),
                            System::Drawing::Drawing2D::LinearGradientMode::Vertical
                        );
                    g->FillRectangle(brush, 0, 0, this->Width, this->Height);
                    backgroundPicture->Image = bmp;
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Error loading background: " + ex->Message);
            }
        }

        void btnUpload_Click(Object^ sender, EventArgs^ e)
        {
            OpenFileDialog^ ofd = gcnew OpenFileDialog();
            ofd->Filter = L"Image Files|*.jpg;*.jpeg;*.png;*.gif;*.bmp;*.tiff;*.ico";
            ofd->Multiselect = true;
            ofd->Title = L"Select Photos to Upload";

            if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            {
                int successCount = 0;
                for each (String ^ fullPath in ofd->FileNames)
                {
                    String^ name = Path::GetFileName(fullPath);
                    int oldSize = photoList->getSize();
                    photoList->addPhoto(fullPath, name, photoListBox);
                    if (photoList->getSize() > oldSize)
                        successCount++;
                }

                UpdatePhotoCount();

                MessageBox::Show(
                    String::Format(L"Successfully uploaded {0} photo(s)!", successCount),
                    L"✅ Upload Complete",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information
                );

                if (photoList->getSize() > 0 && photoDisplay->Image == nullptr)
                {
                    photoListBox->SelectedIndex = 0;
                    DisplayCurrentPhoto();
                }
            }
        }

        void btnDelete_Click(Object^ sender, EventArgs^ e)
        {
            if (photoListBox->SelectedIndex == -1)
            {
                MessageBox::Show(
                    L"Please select a photo to delete.",
                    L"⚠️ No Selection",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Warning
                );
                return;
            }

            photoList->removePhoto(photoListBox);
            photoDisplay->Image = nullptr;
            lblPhotoName->Text = L"";
            UpdatePhotoCount();
        }

        void photoListBox_DoubleClick(Object^ sender, EventArgs^ e)
        {
            if (photoListBox->SelectedIndex >= 0)
            {
                photoList->setCurrentNode(photoListBox->SelectedIndex);
                DisplayCurrentPhoto();
            }
        }

        void btnNext_Click(Object^ sender, EventArgs^ e)
        {
            String^ nextPath = photoList->nextPhoto();
            if (nextPath != nullptr)
            {
                DisplayCurrentPhoto();
            }
        }

        void btnPrevious_Click(Object^ sender, EventArgs^ e)
        {
            String^ prevPath = photoList->prevPhoto();
            if (prevPath != nullptr)
            {
                DisplayCurrentPhoto();
            }
        }

        void DisplayCurrentPhoto()
        {
            String^ path = photoList->getCurrentNodePath();
            if (path != nullptr && File::Exists(path))
            {
                try
                {
                    photoDisplay->Image = Image::FromFile(path);
                    lblPhotoName->Text = photoList->getCurrentNodeName();
                }
                catch (Exception^ ex)
                {
                    MessageBox::Show(
                        L"Error loading image: " + ex->Message,
                        L"❌ Error",
                        MessageBoxButtons::OK,
                        MessageBoxIcon::Error
                    );
                }
            }
        }

        void UpdatePhotoCount()
        {
            int count = photoList->getSize();
            if (count == 0)
                lblCount->Text = L"No photos uploaded";
            else if (count == 1)
                lblCount->Text = L"1 photo in gallery";
            else
                lblCount->Text = String::Format(L"{0} photos in gallery", count);
        }
    };
}