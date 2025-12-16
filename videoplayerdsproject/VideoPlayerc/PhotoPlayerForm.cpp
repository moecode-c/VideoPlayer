#include "PhotoPlayerForm.h"

using namespace VideoPlayerc;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::IO;

// Constructor
PhotoPlayerForm::PhotoPlayerForm(void)
{
    InitializeComponent();
    photoList = gcnew PhotoPlayer::PhotoLinkedList();
    isSlideshowRunning = false;
    currentZoom = 1.0f;
    currentRotation = 0;
    originalImage = nullptr;
    currentImage = nullptr;
    // Initialize panning variables
    isPanning = false;
    panStartPoint = Point(0, 0);
    imageLocation = Point(0, 0);
    pictureBox->SizeMode = PictureBoxSizeMode::Zoom;
    UpdateNavigationButtons();
}

// Destructor
PhotoPlayerForm::~PhotoPlayerForm()
{
    if (components)
        delete components;
    if (slideshowTimer)
        delete slideshowTimer;
    if (originalImage != nullptr)
    {
        delete originalImage;
        originalImage = nullptr;
    }
    if (currentImage != nullptr)
    {
        delete currentImage;
        currentImage = nullptr;
    }
}

// Helper Methods
void PhotoPlayerForm::DisplayPhoto(int index)
{
    String^ photoPath = photoList->GetPhotoAt(index);
    if (photoPath != nullptr && System::IO::File::Exists(photoPath))
    {
        try
        {
            // Clean up previous images
            if (originalImage != nullptr)
            {
                delete originalImage;
                originalImage = nullptr;
            }
            if (currentImage != nullptr)
            {
                delete currentImage;
                currentImage = nullptr;
            }

            // Load new image
            originalImage = safe_cast<Bitmap^>(Image::FromFile(photoPath));

            // Reset zoom and rotation
            currentRotation = 0;
            currentZoom = 1.0f;

            // Display the image
            pictureBox->SizeMode = PictureBoxSizeMode::Zoom;
            pictureBox->Image = safe_cast<Bitmap^>(originalImage->Clone());

            UpdatePhotoInfo();
        }
        catch (Exception^ ex)
        {
            MessageBox::Show(
                String::Format(L"Error loading image: {0}", ex->Message),
                L"Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
        }
    }
}



void PhotoPlayerForm::UpdatePhotoInfo()
{
    if (listBoxPhotos->SelectedIndex >= 0)
    {
        String^ fileName = listBoxPhotos->Items[listBoxPhotos->SelectedIndex]->ToString();
        lblCurrentPhoto->Text = String::Format(
            L"{0} ({1}/{2}) - Zoom: {3}% - Rotation: {4}°",
            fileName,
            listBoxPhotos->SelectedIndex + 1,
            photoList->Count,
            (int)(currentZoom * 100),
            currentRotation
        );
    }
}

void PhotoPlayerForm::ApplyZoom()
{
    if (originalImage == nullptr)
        return;

    try
    {
        int newWidth = (int)(originalImage->Width * currentZoom);
        int newHeight = (int)(originalImage->Height * currentZoom);

        Bitmap^ zoomedBitmap = gcnew Bitmap(newWidth, newHeight);
        Graphics^ g = Graphics::FromImage(zoomedBitmap);

        g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::HighQualityBicubic;
        g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::HighQuality;
        g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::HighQuality;

        g->DrawImage(originalImage, 0, 0, newWidth, newHeight);
        delete g;

        if (pictureBox->Image != nullptr && pictureBox->Image != originalImage)
        {
            delete pictureBox->Image;
        }

        pictureBox->Image = zoomedBitmap;

        if (currentZoom > 1.0f)
        {
            pictureBox->SizeMode = PictureBoxSizeMode::CenterImage;
            pictureBox->Cursor = Cursors::Hand;
        }
        else
        {
            pictureBox->SizeMode = PictureBoxSizeMode::Zoom;
            pictureBox->Cursor = Cursors::Default;
            imageLocation = Point(0, 0);
        }

        pictureBox->Refresh();
    }
    catch (Exception^ ex)
    {
        MessageBox::Show(
            String::Format(L"Error applying zoom: {0}", ex->Message),
            L"Zoom Error",
            MessageBoxButtons::OK,
            MessageBoxIcon::Error
        );
    }
}
void PhotoPlayerForm::UpdateNavigationButtons()
{
    if (!btnPrevious || !btnNext || !btnRotateLeft || !btnRotateRight ||
        !btnZoomIn || !btnZoomOut || !btnResetZoom || !btnSlideshow ||
        !btnDelete || !btnSortByName || !btnReverse || !btnFirst || !btnLast ||
        !listBoxPhotos || !photoList)
        return;

    bool hasPhotos = listBoxPhotos->Items->Count > 0;
    bool hasSelection = listBoxPhotos->SelectedIndex >= 0;

    btnPrevious->Enabled = hasSelection && listBoxPhotos->SelectedIndex > 0;
    btnNext->Enabled = hasSelection && listBoxPhotos->SelectedIndex < listBoxPhotos->Items->Count - 1;
    btnRotateLeft->Enabled = hasSelection;
    btnRotateRight->Enabled = hasSelection;
    btnZoomIn->Enabled = hasSelection;
    btnZoomOut->Enabled = hasSelection;
    btnResetZoom->Enabled = hasSelection;
    btnSlideshow->Enabled = hasPhotos;
    btnDelete->Enabled = hasSelection;
    btnSortByName->Enabled = hasPhotos;
    btnReverse->Enabled = hasPhotos;
    btnFirst->Enabled = hasPhotos;
    btnLast->Enabled = hasPhotos;
}

// PART 2: InitializeComponent - First Half
// Add this after PART 1

// Replace the Form initialization section at the beginning of InitializeComponent
// This applies the beautiful navy blue theme

void PhotoPlayerForm::InitializeComponent(void)
{
    this->SuspendLayout();

    // Form styling with navy blue theme
    this->Text = L"Photo Player - Single Linked List Edition";
    this->Size = System::Drawing::Size(1100, 700);
    this->StartPosition = FormStartPosition::CenterScreen;
    this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
    this->MaximizeBox = false;

    // BEAUTIFUL NAVY BLUE BACKGROUND
    this->BackColor = Color::FromArgb(25, 42, 86);  // Deep navy blue

    // PictureBox with navy theme
    this->pictureBox = gcnew System::Windows::Forms::PictureBox();
    this->pictureBox->Location = System::Drawing::Point(20, 20);
    this->pictureBox->Size = System::Drawing::Size(600, 500);
    this->pictureBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
    this->pictureBox->SizeMode = PictureBoxSizeMode::Zoom;
    this->pictureBox->BackColor = Color::FromArgb(35, 55, 110);  // Lighter navy for contrast
    this->pictureBox->BackColor = Color::FromArgb(35, 55, 110);
    this->pictureBox->MouseDown += gcnew MouseEventHandler(this, &PhotoPlayerForm::pictureBox_MouseDown);
    this->pictureBox->MouseMove += gcnew MouseEventHandler(this, &PhotoPlayerForm::pictureBox_MouseMove);
    this->pictureBox->MouseUp += gcnew MouseEventHandler(this, &PhotoPlayerForm::pictureBox_MouseUp);
    this->Controls->Add(this->pictureBox);
    this->Controls->Add(this->pictureBox);

    // Current Photo Label with white text on navy
    this->lblCurrentPhoto = gcnew System::Windows::Forms::Label();
    this->lblCurrentPhoto->Location = System::Drawing::Point(20, 530);
    this->lblCurrentPhoto->Size = System::Drawing::Size(600, 25);
    this->lblCurrentPhoto->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->lblCurrentPhoto->TextAlign = ContentAlignment::MiddleCenter;
    this->lblCurrentPhoto->Text = L"No photo selected";
    this->lblCurrentPhoto->BackColor = Color::Transparent;
    this->lblCurrentPhoto->ForeColor = Color::White;  // White text on navy
    this->Controls->Add(this->lblCurrentPhoto);

    // Navigation Buttons with navy-themed colors
    int navButtonY = 560;
    int navButtonWidth = 140;
    int navButtonHeight = 35;

    this->btnPrevious = gcnew System::Windows::Forms::Button();
    this->btnPrevious->Text = L"< Previous";
    this->btnPrevious->Location = System::Drawing::Point(20, navButtonY);
    this->btnPrevious->Size = System::Drawing::Size(navButtonWidth, navButtonHeight);
    this->btnPrevious->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnPrevious->BackColor = Color::FromArgb(52, 152, 219);  // Bright blue
    this->btnPrevious->ForeColor = Color::White;
    this->btnPrevious->FlatStyle = FlatStyle::Flat;
    this->btnPrevious->FlatAppearance->BorderColor = Color::FromArgb(41, 128, 185);
    this->btnPrevious->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnPrevious_Click);
    this->Controls->Add(this->btnPrevious);

    this->btnSlideshow = gcnew System::Windows::Forms::Button();
    this->btnSlideshow->Text = L"Play Slideshow";
    this->btnSlideshow->Location = System::Drawing::Point(170, navButtonY);
    this->btnSlideshow->Size = System::Drawing::Size(navButtonWidth, navButtonHeight);
    this->btnSlideshow->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnSlideshow->BackColor = Color::FromArgb(46, 204, 113);  // Green
    this->btnSlideshow->ForeColor = Color::White;
    this->btnSlideshow->FlatStyle = FlatStyle::Flat;
    this->btnSlideshow->FlatAppearance->BorderColor = Color::FromArgb(39, 174, 96);
    this->btnSlideshow->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnSlideshow_Click);
    this->Controls->Add(this->btnSlideshow);

    this->btnNext = gcnew System::Windows::Forms::Button();
    this->btnNext->Text = L"Next >";
    this->btnNext->Location = System::Drawing::Point(320, navButtonY);
    this->btnNext->Size = System::Drawing::Size(navButtonWidth, navButtonHeight);
    this->btnNext->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnNext->BackColor = Color::FromArgb(52, 152, 219);  // Bright blue
    this->btnNext->ForeColor = Color::White;
    this->btnNext->FlatStyle = FlatStyle::Flat;
    this->btnNext->FlatAppearance->BorderColor = Color::FromArgb(41, 128, 185);
    this->btnNext->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnNext_Click);
    this->Controls->Add(this->btnNext);

    // Slideshow Speed Control with white text
    this->lblSpeed = gcnew System::Windows::Forms::Label();
    this->lblSpeed->Location = System::Drawing::Point(470, navButtonY);
    this->lblSpeed->Size = System::Drawing::Size(100, 20);       // NEW - matches trackbar
    this->lblSpeed->Font = gcnew System::Drawing::Font(L"Arial", 9);
    this->lblSpeed->Text = L"Speed: 3 sec";
    this->lblSpeed->BackColor = Color::Transparent;
    this->lblSpeed->ForeColor = Color::White;  // White text on navy
    this->Controls->Add(this->lblSpeed);

    this->trackBarSpeed = gcnew System::Windows::Forms::TrackBar();
    this->trackBarSpeed->Location = System::Drawing::Point(470, navButtonY + 20);
    this->trackBarSpeed->Size = System::Drawing::Size(100, 30);
    this->trackBarSpeed->Minimum = 1;
    this->trackBarSpeed->Maximum = 10;
    this->trackBarSpeed->Value = 3;
    this->trackBarSpeed->TickFrequency = 1;
    this->trackBarSpeed->BackColor = Color::FromArgb(25, 42, 86);  // Match background
    this->trackBarSpeed->Scroll += gcnew EventHandler(this, &PhotoPlayerForm::trackBarSpeed_Scroll);
    this->Controls->Add(this->trackBarSpeed);

    // Rotation and Zoom Buttons
    int rotateY = 605;

    this->btnRotateLeft = gcnew System::Windows::Forms::Button();
    this->btnRotateLeft->Text = L"↶ Rotate Left";
    this->btnRotateLeft->Location = System::Drawing::Point(20, rotateY);
    this->btnRotateLeft->Size = System::Drawing::Size(140, 35);
    this->btnRotateLeft->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnRotateLeft->BackColor = Color::FromArgb(241, 196, 15);  // Gold
    this->btnRotateLeft->ForeColor = Color::FromArgb(44, 62, 80);  // Dark text
    this->btnRotateLeft->FlatStyle = FlatStyle::Flat;
    this->btnRotateLeft->FlatAppearance->BorderColor = Color::FromArgb(243, 156, 18);
    this->btnRotateLeft->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnRotateLeft_Click);
    this->Controls->Add(this->btnRotateLeft);

    this->btnRotateRight = gcnew System::Windows::Forms::Button();
    this->btnRotateRight->Text = L"↷ Rotate Right";
    this->btnRotateRight->Location = System::Drawing::Point(170, rotateY);
    this->btnRotateRight->Size = System::Drawing::Size(140, 35);
    this->btnRotateRight->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnRotateRight->BackColor = Color::FromArgb(241, 196, 15);  // Gold
    this->btnRotateRight->ForeColor = Color::FromArgb(44, 62, 80);  // Dark text
    this->btnRotateRight->FlatStyle = FlatStyle::Flat;
    this->btnRotateRight->FlatAppearance->BorderColor = Color::FromArgb(243, 156, 18);
    this->btnRotateRight->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnRotateRight_Click);
    this->Controls->Add(this->btnRotateRight);

    this->btnZoomIn = gcnew System::Windows::Forms::Button();
    this->btnZoomIn->Text = L"+ Zoom In";
    this->btnZoomIn->Location = System::Drawing::Point(320, rotateY);
    this->btnZoomIn->Size = System::Drawing::Size(95, 35);
    this->btnZoomIn->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnZoomIn->BackColor = Color::FromArgb(155, 89, 182);  // Purple
    this->btnZoomIn->ForeColor = Color::White;
    this->btnZoomIn->FlatStyle = FlatStyle::Flat;
    this->btnZoomIn->FlatAppearance->BorderColor = Color::FromArgb(142, 68, 173);
    this->btnZoomIn->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnZoomIn_Click);
    this->Controls->Add(this->btnZoomIn);

    this->btnZoomOut = gcnew System::Windows::Forms::Button();
    this->btnZoomOut->Text = L"- Zoom Out";
    this->btnZoomOut->Location = System::Drawing::Point(425, rotateY);
    this->btnZoomOut->Size = System::Drawing::Size(95, 35);
    this->btnZoomOut->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnZoomOut->BackColor = Color::FromArgb(155, 89, 182);  // Purple
    this->btnZoomOut->ForeColor = Color::White;
    this->btnZoomOut->FlatStyle = FlatStyle::Flat;
    this->btnZoomOut->FlatAppearance->BorderColor = Color::FromArgb(142, 68, 173);
    this->btnZoomOut->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnZoomOut_Click);
    this->Controls->Add(this->btnZoomOut);

    this->btnResetZoom = gcnew System::Windows::Forms::Button();
    this->btnResetZoom->Text = L"Reset";
    this->btnResetZoom->Location = System::Drawing::Point(530, rotateY);
    this->btnResetZoom->Size = System::Drawing::Size(90, 35);
    this->btnResetZoom->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnResetZoom->BackColor = Color::FromArgb(155, 89, 182);  // Purple
    this->btnResetZoom->ForeColor = Color::White;
    this->btnResetZoom->FlatStyle = FlatStyle::Flat;
    this->btnResetZoom->FlatAppearance->BorderColor = Color::FromArgb(142, 68, 173);
    this->btnResetZoom->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnResetZoom_Click);
    this->Controls->Add(this->btnResetZoom);

    // First/Last buttons
    this->btnFirst = gcnew System::Windows::Forms::Button();
    this->btnFirst->Text = L"|< First";
    this->btnFirst->Location = System::Drawing::Point(470, 560);
    this->btnFirst->Size = System::Drawing::Size(75, 35);
    this->btnFirst->Font = gcnew System::Drawing::Font(L"Arial", 9, FontStyle::Bold);
    this->btnFirst->BackColor = Color::FromArgb(52, 152, 219);
    this->btnFirst->ForeColor = Color::White;
    this->btnFirst->FlatStyle = FlatStyle::Flat;
    this->btnFirst->FlatAppearance->BorderColor = Color::FromArgb(41, 128, 185);
    this->btnFirst->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnFirst_Click);
    this->Controls->Add(this->btnFirst);

    this->btnLast = gcnew System::Windows::Forms::Button();
    this->btnLast->Text = L"Last >|";
    this->btnLast->Location = System::Drawing::Point(545, 560);
    this->btnLast->Size = System::Drawing::Size(75, 35);
    this->btnLast->Font = gcnew System::Drawing::Font(L"Arial", 9, FontStyle::Bold);
    this->btnLast->BackColor = Color::FromArgb(52, 152, 219);
    this->btnLast->ForeColor = Color::White;
    this->btnLast->FlatStyle = FlatStyle::Flat;
    this->btnLast->FlatAppearance->BorderColor = Color::FromArgb(41, 128, 185);
    this->btnLast->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnLast_Click);
    this->Controls->Add(this->btnLast);

    // RIGHT PANEL - Continue with rest of components...
    // (Add the right panel code from Task 1 here with navy theme updates)

    int rightX = 650;

    // Back to Menu Button
    this->btnBackToMenu = gcnew System::Windows::Forms::Button();
    this->btnBackToMenu->BackColor = System::Drawing::Color::FromArgb(231, 76, 60);  // Red
    this->btnBackToMenu->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    this->btnBackToMenu->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
    this->btnBackToMenu->ForeColor = System::Drawing::Color::White;
    this->btnBackToMenu->Location = System::Drawing::Point(rightX, 20);
    this->btnBackToMenu->Size = System::Drawing::Size(120, 40);
    this->btnBackToMenu->Text = L"← Back";
    this->btnBackToMenu->FlatAppearance->BorderColor = Color::FromArgb(192, 57, 43);
    this->btnBackToMenu->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnBackToMenu_Click);
    this->Controls->Add(this->btnBackToMenu);

    this->btnUpload = gcnew System::Windows::Forms::Button();
    this->btnUpload->Text = L"📤 Upload Photos";
    this->btnUpload->Location = System::Drawing::Point(rightX, 70);
    this->btnUpload->Size = System::Drawing::Size(400, 45);
    this->btnUpload->Font = gcnew System::Drawing::Font(L"Arial", 12, FontStyle::Bold);
    this->btnUpload->BackColor = Color::FromArgb(52, 152, 219);
    this->btnUpload->ForeColor = Color::White;
    this->btnUpload->FlatStyle = FlatStyle::Flat;
    this->btnUpload->FlatAppearance->BorderColor = Color::FromArgb(41, 128, 185);
    this->btnUpload->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnUpload_Click);
    this->Controls->Add(this->btnUpload);

    this->txtSearch = gcnew System::Windows::Forms::TextBox();
    this->txtSearch->Location = System::Drawing::Point(rightX, 130);
    this->txtSearch->Size = System::Drawing::Size(250, 25);
    this->txtSearch->Font = gcnew System::Drawing::Font(L"Arial", 10);
    this->Controls->Add(this->txtSearch);

    this->btnSearch = gcnew System::Windows::Forms::Button();
    this->btnSearch->Text = L"🔍";
    this->btnSearch->Location = System::Drawing::Point(rightX + 260, 128);
    this->btnSearch->Size = System::Drawing::Size(60, 30);
    this->btnSearch->Font = gcnew System::Drawing::Font(L"Arial", 12);
    this->btnSearch->BackColor = Color::FromArgb(52, 152, 219);
    this->btnSearch->ForeColor = Color::White;
    this->btnSearch->FlatStyle = FlatStyle::Flat;
    this->btnSearch->FlatAppearance->BorderColor = Color::FromArgb(41, 128, 185);
    this->btnSearch->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnSearch_Click);
    this->Controls->Add(this->btnSearch);

    this->btnClearSearch = gcnew System::Windows::Forms::Button();
    this->btnClearSearch->Text = L"Clear";
    this->btnClearSearch->Location = System::Drawing::Point(rightX + 330, 128);
    this->btnClearSearch->Size = System::Drawing::Size(60, 30);
    this->btnClearSearch->Font = gcnew System::Drawing::Font(L"Arial", 9);
    this->btnClearSearch->BackColor = Color::FromArgb(149, 165, 166);
    this->btnClearSearch->ForeColor = Color::White;
    this->btnClearSearch->FlatStyle = FlatStyle::Flat;
    this->btnClearSearch->FlatAppearance->BorderColor = Color::FromArgb(127, 140, 141);
    this->btnClearSearch->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnClearSearch_Click);
    this->Controls->Add(this->btnClearSearch);

    this->btnSortByName = gcnew System::Windows::Forms::Button();
    this->btnSortByName->Text = L"Sort A-Z";
    this->btnSortByName->Location = System::Drawing::Point(rightX, 170);
    this->btnSortByName->Size = System::Drawing::Size(400, 35);
    this->btnSortByName->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnSortByName->BackColor = Color::FromArgb(155, 89, 182);
    this->btnSortByName->ForeColor = Color::White;
    this->btnSortByName->FlatStyle = FlatStyle::Flat;
    this->btnSortByName->FlatAppearance->BorderColor = Color::FromArgb(142, 68, 173);
    this->btnSortByName->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnSortByName_Click);
    this->Controls->Add(this->btnSortByName);

    this->btnReverse = gcnew System::Windows::Forms::Button();
    this->btnReverse->Text = L"⇅ Reverse Order";
    this->btnReverse->Location = System::Drawing::Point(rightX, 210);
    this->btnReverse->Size = System::Drawing::Size(400, 35);
    this->btnReverse->Font = gcnew System::Drawing::Font(L"Arial", 10, FontStyle::Bold);
    this->btnReverse->BackColor = Color::FromArgb(52, 73, 94);
    this->btnReverse->ForeColor = Color::White;
    this->btnReverse->FlatStyle = FlatStyle::Flat;
    this->btnReverse->FlatAppearance->BorderColor = Color::FromArgb(44, 62, 80);
    this->btnReverse->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnReverse_Click);
    this->Controls->Add(this->btnReverse);

    this->listBoxPhotos = gcnew System::Windows::Forms::ListBox();
    this->listBoxPhotos->Location = System::Drawing::Point(rightX, 250);
    this->listBoxPhotos->Size = System::Drawing::Size(400, 300);
    this->listBoxPhotos->Font = gcnew System::Drawing::Font(L"Arial", 10);
    this->listBoxPhotos->BackColor = Color::FromArgb(236, 240, 241);
    this->listBoxPhotos->SelectedIndexChanged += gcnew EventHandler(this, &PhotoPlayerForm::listBoxPhotos_SelectedIndexChanged);
    this->Controls->Add(this->listBoxPhotos);

    this->btnDelete = gcnew System::Windows::Forms::Button();
    this->btnDelete->Text = L"🗑️ Delete Selected";
    this->btnDelete->Location = System::Drawing::Point(rightX, 560);
    this->btnDelete->Size = System::Drawing::Size(400, 45);
    this->btnDelete->Font = gcnew System::Drawing::Font(L"Arial", 12, FontStyle::Bold);
    this->btnDelete->BackColor = Color::FromArgb(231, 76, 60);
    this->btnDelete->ForeColor = Color::White;
    this->btnDelete->FlatStyle = FlatStyle::Flat;
    this->btnDelete->FlatAppearance->BorderColor = Color::FromArgb(192, 57, 43);
    this->btnDelete->Click += gcnew EventHandler(this, &PhotoPlayerForm::btnDelete_Click);
    this->Controls->Add(this->btnDelete);

    this->lblInfo = gcnew System::Windows::Forms::Label();
    this->lblInfo->Location = System::Drawing::Point(rightX, 620);
    this->lblInfo->Size = System::Drawing::Size(400, 30);
    this->lblInfo->Font = gcnew System::Drawing::Font(L"Arial", 11, FontStyle::Bold);
    this->lblInfo->Text = L"Total Photos: 0";
    this->lblInfo->TextAlign = ContentAlignment::MiddleCenter;
    this->lblInfo->BackColor = Color::Transparent;
    this->lblInfo->ForeColor = Color::White;  // White text on navy
    this->Controls->Add(this->lblInfo);

    this->slideshowTimer = gcnew System::Windows::Forms::Timer();
    this->slideshowTimer->Interval = 3000;
    this->slideshowTimer->Tick += gcnew EventHandler(this, &PhotoPlayerForm::slideshowTimer_Tick);

    this->ResumeLayout(false);
}
// FIX ERRORS IN PART 3 ABOVE



// PART 4: Event Handlers - First Set
// Add this after PART 3

System::Void PhotoPlayerForm::btnUpload_Click(System::Object^ sender, System::EventArgs^ e)
{
    OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
    openFileDialog->Filter = L"Image Files|*.jpg;*.jpeg;*.png;*.bmp;*.gif";
    openFileDialog->Title = L"Select Photos";
    openFileDialog->Multiselect = true;

    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        for each (String ^ filePath in openFileDialog->FileNames)
        {
            photoList->AddPhoto(filePath);
            listBoxPhotos->Items->Add(System::IO::Path::GetFileName(filePath));
        }
        lblInfo->Text = String::Format(L"Total Photos: {0}", photoList->Count);
        MessageBox::Show(
            String::Format(L"{0} photo(s) uploaded successfully!", openFileDialog->FileNames->Length),
            L"Success",
            MessageBoxButtons::OK,
            MessageBoxIcon::Information
        );
        UpdateNavigationButtons();
    }
}

System::Void PhotoPlayerForm::listBoxPhotos_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->SelectedIndex >= 0)
    {
        DisplayPhoto(listBoxPhotos->SelectedIndex);
        UpdateNavigationButtons();
    }
}

System::Void PhotoPlayerForm::btnDelete_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->SelectedIndex < 0)
    {
        MessageBox::Show(L"Please select a photo to delete.",
            L"No Selection", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }
    int selectedIndex = listBoxPhotos->SelectedIndex;
    System::Windows::Forms::DialogResult result = MessageBox::Show(
        L"Are you sure you want to delete this photo from the list?",
        L"Confirm Delete",
        MessageBoxButtons::YesNo,
        MessageBoxIcon::Question
    );
    if (result == System::Windows::Forms::DialogResult::Yes)
    {
        if (photoList->DeletePhotoAt(selectedIndex))
        {
            listBoxPhotos->Items->RemoveAt(selectedIndex);
            pictureBox->Image = nullptr;
            lblInfo->Text = String::Format(L"Total Photos: {0}", photoList->Count);
            lblCurrentPhoto->Text = L"No photo selected";
            MessageBox::Show(L"Photo deleted successfully!",
                L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            UpdateNavigationButtons();
        }
    }
}

System::Void PhotoPlayerForm::btnPrevious_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->SelectedIndex > 0)
        listBoxPhotos->SelectedIndex--;
}

System::Void PhotoPlayerForm::btnNext_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->SelectedIndex < listBoxPhotos->Items->Count - 1)
        listBoxPhotos->SelectedIndex++;
}

System::Void PhotoPlayerForm::btnFirst_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (photoList->Count > 0)
        listBoxPhotos->SelectedIndex = 0;
}

System::Void PhotoPlayerForm::btnLast_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (photoList->Count > 0)
        listBoxPhotos->SelectedIndex = photoList->Count - 1;
}

System::Void PhotoPlayerForm::btnRotateLeft_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (pictureBox->Image != nullptr)
    {
        pictureBox->Image->RotateFlip(RotateFlipType::Rotate270FlipNone);
        pictureBox->Refresh();
        currentRotation = (currentRotation - 90 + 360) % 360;
        if (originalImage != nullptr)
        {
            originalImage->RotateFlip(RotateFlipType::Rotate270FlipNone);
        }
        if (currentZoom != 1.0f)
        {
            ApplyZoom();
        }
        UpdatePhotoInfo();
    }
}

System::Void PhotoPlayerForm::btnRotateRight_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (pictureBox->Image != nullptr)
    {
        pictureBox->Image->RotateFlip(RotateFlipType::Rotate90FlipNone);
        pictureBox->Refresh();
        currentRotation = (currentRotation + 90) % 360;
        if (originalImage != nullptr)
        {
            originalImage->RotateFlip(RotateFlipType::Rotate90FlipNone);
        }
        if (currentZoom != 1.0f)
        {
            ApplyZoom();
        }
        UpdatePhotoInfo();
    }
}

System::Void PhotoPlayerForm::btnZoomIn_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (originalImage != nullptr && currentZoom < 3.0f)
    {
        currentZoom += 0.25f;
        ApplyZoom();
        UpdatePhotoInfo();
    }
    else if (currentZoom >= 3.0f)
    {
        MessageBox::Show(L"Maximum zoom level reached (300%)", L"Zoom Limit",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
}
System::Void PhotoPlayerForm::btnZoomOut_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (originalImage != nullptr && currentZoom > 0.5f)
    {
        currentZoom -= 0.25f;
        ApplyZoom();
        UpdatePhotoInfo();
    }
    else if (currentZoom <= 0.5f)
    {
        MessageBox::Show(L"Minimum zoom level reached (50%)", L"Zoom Limit",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
}

System::Void PhotoPlayerForm::btnResetZoom_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->SelectedIndex >= 0 && originalImage != nullptr)
    {
        currentZoom = 1.0f;
        currentRotation = 0;

        isPanning = false;
        imageLocation = Point(0, 0);

        if (pictureBox->Image != nullptr && pictureBox->Image != originalImage)
        {
            delete pictureBox->Image;
        }

        pictureBox->SizeMode = PictureBoxSizeMode::Zoom;
        pictureBox->Cursor = Cursors::Default;
        pictureBox->Image = safe_cast<Bitmap^>(originalImage->Clone());
        pictureBox->Refresh();

        UpdatePhotoInfo();
    }
}

// PART 5: Event Handlers - Final Set
// Add this after PART 4

System::Void PhotoPlayerForm::btnSlideshow_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (photoList->Count == 0)
    {
        MessageBox::Show(L"Please upload photos first!", L"No Photos",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }
    if (!isSlideshowRunning)
    {
        isSlideshowRunning = true;
        btnSlideshow->Text = L"Stop Slideshow";
        btnSlideshow->BackColor = Color::FromArgb(220, 53, 69);
        slideshowTimer->Start();
        if (listBoxPhotos->SelectedIndex < 0)
        {
            listBoxPhotos->SelectedIndex = 0;
        }
    }
    else
    {
        isSlideshowRunning = false;
        btnSlideshow->Text = L"Play Slideshow";
        btnSlideshow->BackColor = Color::FromArgb(40, 167, 69);
        slideshowTimer->Stop();
    }
}

System::Void PhotoPlayerForm::slideshowTimer_Tick(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->SelectedIndex < listBoxPhotos->Items->Count - 1)
    {
        listBoxPhotos->SelectedIndex++;
    }
    else
    {
        listBoxPhotos->SelectedIndex = 0;
    }
}

System::Void PhotoPlayerForm::trackBarSpeed_Scroll(System::Object^ sender, System::EventArgs^ e)
{
    int speed = trackBarSpeed->Value;
    slideshowTimer->Interval = speed * 1000;
    lblSpeed->Text = String::Format(L"Speed: {0} sec", speed);
}

System::Void PhotoPlayerForm::btnSortByName_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->Items->Count > 0)
    {
        array<String^>^ items = gcnew array<String^>(listBoxPhotos->Items->Count);
        for (int i = 0; i < listBoxPhotos->Items->Count; i++)
        {
            items[i] = listBoxPhotos->Items[i]->ToString();
        }
        Array::Sort(items);
        listBoxPhotos->Items->Clear();
        for each (String ^ item in items)
        {
            listBoxPhotos->Items->Add(item);
        }
        MessageBox::Show(L"Photos sorted alphabetically!", L"Sorted",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
}

System::Void PhotoPlayerForm::btnReverse_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (photoList->Count <= 1)
        return;
    photoList->Reverse();
    listBoxPhotos->Items->Clear();
    for (int i = 0; i < photoList->Count; i++)
    {
        String^ photoPath = photoList->GetPhotoAt(i);
        listBoxPhotos->Items->Add(System::IO::Path::GetFileName(photoPath));
    }
    MessageBox::Show(L"Photo order reversed!", L"Reversed",
        MessageBoxButtons::OK, MessageBoxIcon::Information);
}

System::Void PhotoPlayerForm::btnSearch_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ searchTerm = txtSearch->Text->Trim()->ToLower();
    if (String::IsNullOrEmpty(searchTerm))
    {
        MessageBox::Show(L"Please enter a search term!", L"Empty Search",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }
    listBoxPhotos->Items->Clear();
    int found = 0;
    for (int i = 0; i < photoList->Count; i++)
    {
        String^ photoPath = photoList->GetPhotoAt(i);
        String^ fileName = System::IO::Path::GetFileName(photoPath)->ToLower();
        if (fileName->Contains(searchTerm))
        {
            listBoxPhotos->Items->Add(System::IO::Path::GetFileName(photoPath));
            found++;
        }
    }
    MessageBox::Show(String::Format(L"Found {0} photo(s) matching '{1}'",
        found, searchTerm), L"Search Results",
        MessageBoxButtons::OK, MessageBoxIcon::Information);
}

System::Void PhotoPlayerForm::btnClearSearch_Click(System::Object^ sender, System::EventArgs^ e)
{
    txtSearch->Text = L"";
    listBoxPhotos->Items->Clear();
    for (int i = 0; i < photoList->Count; i++)
    {
        String^ photoPath = photoList->GetPhotoAt(i);
        listBoxPhotos->Items->Add(System::IO::Path::GetFileName(photoPath));
    }
    UpdateNavigationButtons();
}

// NEW - Back to Menu Button Handler
System::Void PhotoPlayerForm::btnBackToMenu_Click(System::Object^ sender, System::EventArgs^ e)
{
    // Stop slideshow if running
    if (isSlideshowRunning)
    {
        isSlideshowRunning = false;
        slideshowTimer->Stop();
    }

    // Close this form to return to menu
    this->Close();
}

System::Void PhotoPlayerForm::pictureBox_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
    if (currentZoom > 1.0f && e->Button == System::Windows::Forms::MouseButtons::Left)
    {
        isPanning = true;
        panStartPoint = Point(e->X - imageLocation.X, e->Y - imageLocation.Y);
        pictureBox->Cursor = Cursors::SizeAll;
    }
}

System::Void PhotoPlayerForm::pictureBox_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
    if (isPanning)
    {
        imageLocation = Point(e->X - panStartPoint.X, e->Y - panStartPoint.Y);

        if (pictureBox->Image != nullptr)
        {
            Bitmap^ pannedBitmap = gcnew Bitmap(pictureBox->Width, pictureBox->Height);
            Graphics^ g = Graphics::FromImage(pannedBitmap);

            g->Clear(pictureBox->BackColor);
            g->DrawImage(pictureBox->Image, imageLocation);

            delete g;

            Bitmap^ oldImage = safe_cast<Bitmap^>(pictureBox->Image);
            pictureBox->Image = pannedBitmap;
            pictureBox->SizeMode = PictureBoxSizeMode::Normal;
            pictureBox->Refresh();
        }
    }
}

System::Void PhotoPlayerForm::pictureBox_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
    if (isPanning)
    {
        isPanning = false;
        pictureBox->Cursor = (currentZoom > 1.0f) ? Cursors::Hand : Cursors::Default;
    }
}

