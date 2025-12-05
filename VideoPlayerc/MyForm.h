#pragma once
#include "LinkedList.h"

namespace VideoPlayerc {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::IO;
    using namespace System::Drawing;

    public ref class MainForm : public Form
    {
    private:
        System::Windows::Forms::Button^ btnDelete;
        System::Windows::Forms::Button^ btnUpload;
        System::Windows::Forms::ListBox^ listBox2;
        System::Windows::Forms::PictureBox^ pictureBoxBackground;
        System::Windows::Forms::Button^ btnPlay;
        VideoList^ videoList;

    public:
        MainForm(void)
        {
            InitializeComponent();
            videoList = gcnew VideoList();
            LoadBackgroundImage();
            LoadButtonImages();     // Load button images
            PositionListBox();      // Position listbox on startup
            PositionButtons();      // Position buttons on startup
            AutoLoadPlaylist();     // Auto-load saved playlist on startup
        }

    protected:
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

        // Override FormClosing to auto-save before closing
        virtual void OnFormClosing(FormClosingEventArgs^ e) override
        {
            AutoSavePlaylist();
            Form::OnFormClosing(e);
        }

        // Override OnResize to reposition listbox when form is resized
        virtual void OnResize(EventArgs^ e) override
        {
            Form::OnResize(e);
            PositionListBox();
            PositionButtons();  // Also reposition buttons when form is resized
        }

    private:
        System::ComponentModel::Container^ components;

        void InitializeComponent(void)
        {
            this->btnDelete = (gcnew System::Windows::Forms::Button());
            this->btnUpload = (gcnew System::Windows::Forms::Button());
            this->listBox2 = (gcnew System::Windows::Forms::ListBox());
            this->pictureBoxBackground = (gcnew System::Windows::Forms::PictureBox());
            this->btnPlay = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->BeginInit();
            this->SuspendLayout();
            // 
            // btnDelete
            // 
            this->btnDelete->Location = System::Drawing::Point(279, 342);
            this->btnDelete->Name = L"btnDelete";
            this->btnDelete->Size = System::Drawing::Size(68, 54);
            this->btnDelete->TabIndex = 1;
            this->btnDelete->Text = L"Delete Video";
            this->btnDelete->UseVisualStyleBackColor = true;
            this->btnDelete->Click += gcnew System::EventHandler(this, &MainForm::btnDelete_Click);
            // 
            // btnUpload
            // 
            this->btnUpload->Location = System::Drawing::Point(386, 342);
            this->btnUpload->Name = L"btnUpload";
            this->btnUpload->Size = System::Drawing::Size(70, 63);
            this->btnUpload->TabIndex = 2;
            this->btnUpload->Text = L"Upload Video";
            this->btnUpload->UseVisualStyleBackColor = true;
            this->btnUpload->Click += gcnew System::EventHandler(this, &MainForm::btnUpload_Click);
            // 
            // listBox2
            // 
            this->listBox2->BackColor = System::Drawing::SystemColors::InactiveCaption;
            this->listBox2->FormattingEnabled = true;
            this->listBox2->ItemHeight = 20;
            this->listBox2->Location = System::Drawing::Point(137, 112);
            this->listBox2->Name = L"listBox2";
            this->listBox2->Size = System::Drawing::Size(177, 224);
            this->listBox2->TabIndex = 3;
            // 
            // pictureBoxBackground
            // 
            this->pictureBoxBackground->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxBackground->Location = System::Drawing::Point(0, 0);
            this->pictureBoxBackground->Name = L"pictureBoxBackground";
            this->pictureBoxBackground->Size = System::Drawing::Size(742, 437);
            this->pictureBoxBackground->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->pictureBoxBackground->TabIndex = 0;
            this->pictureBoxBackground->TabStop = false;
            // 
            // btnPlay
            // 
            this->btnPlay->Location = System::Drawing::Point(394, 185);
            this->btnPlay->Name = L"btnPlay";
            this->btnPlay->Size = System::Drawing::Size(61, 60);
            this->btnPlay->TabIndex = 4;
            this->btnPlay->Text = L"button1";
            this->btnPlay->UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this->ClientSize = System::Drawing::Size(742, 437);
            this->Controls->Add(this->btnPlay);
            this->Controls->Add(this->listBox2);
            this->Controls->Add(this->btnUpload);
            this->Controls->Add(this->btnDelete);
            this->Controls->Add(this->pictureBoxBackground);
            this->Name = L"MainForm";
            this->Text = L"Video Player";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->EndInit();
            this->ResumeLayout(false);

        }

        // Position listbox dynamically based on form size
        void PositionListBox()
        {
            // Calculate positions based on percentages of form size
            double topLinePercent = 24.5;
            double bottomLinePercent = 75.0;
            double leftPercent = 33.5;
            double rightMarginPercent = 37.0;

            int topPosition = (int)(this->ClientSize.Height * topLinePercent / 100.0);
            int bottomPosition = (int)(this->ClientSize.Height * bottomLinePercent / 100.0);
            int leftPosition = (int)(this->ClientSize.Width * leftPercent / 100.0);
            int rightMargin = (int)(this->ClientSize.Width * rightMarginPercent / 100.0);

            listBox2->Location = System::Drawing::Point(leftPosition, topPosition);
            listBox2->Size = System::Drawing::Size(
                this->ClientSize.Width - leftPosition - rightMargin,
                bottomPosition - topPosition
            );
        }

        // Position buttons dynamically based on form size
        void PositionButtons()
        {
            double buttonWidthPercent = 9.0;
            double buttonHeightPercent = 12.0;

            int buttonWidth = (int)(this->ClientSize.Width * buttonWidthPercent / 100.0);
            int buttonHeight = (int)(this->ClientSize.Height * buttonHeightPercent / 100.0);

            double buttonRowTopPercent = 78.0;
            int buttonTop = (int)(this->ClientSize.Height * buttonRowTopPercent / 100.0);

            double spacingPercent = 2.0;
            int spacing = (int)(this->ClientSize.Width * spacingPercent / 100.0);

            double startLeftPercent = 32.9;
            int startLeft = (int)(this->ClientSize.Width * startLeftPercent / 100.0);

            // btnDelete - First button
            btnDelete->Location = System::Drawing::Point(startLeft, buttonTop);
            btnDelete->Size = System::Drawing::Size(buttonWidth, buttonHeight);

            // btnPlay - Second button
            int playLeft = startLeft + buttonWidth + spacing;
            btnPlay->Location = System::Drawing::Point(playLeft, buttonTop);
            btnPlay->Size = System::Drawing::Size(buttonWidth, buttonHeight);

            // btnUpload - Third button
            int uploadLeft = playLeft + buttonWidth + spacing;
            btnUpload->Location = System::Drawing::Point(uploadLeft, buttonTop);
            btnUpload->Size = System::Drawing::Size(buttonWidth, buttonHeight);
        }

        // Load button images
        void LoadButtonImages()
        {
            try
            {
                String^ basePath = Application::StartupPath;

                // Load Delete button image
                String^ deleteImagePath = Path::Combine(basePath, "Images\\btndelete.jpg");
                if (File::Exists(deleteImagePath))
                {
                    btnDelete->BackgroundImage = Image::FromFile(deleteImagePath);
                    btnDelete->BackgroundImageLayout = ImageLayout::Stretch;
                    btnDelete->Text = "";

                    // Remove borders
                    btnDelete->FlatStyle = FlatStyle::Flat;
                    btnDelete->FlatAppearance->BorderSize = 0;
                    btnDelete->BackColor = Color::Transparent;
                }

                // Load Upload button image
                String^ uploadImagePath = Path::Combine(basePath, "Images\\btnupload.jpg");
                if (File::Exists(uploadImagePath))
                {
                    btnUpload->BackgroundImage = Image::FromFile(uploadImagePath);
                    btnUpload->BackgroundImageLayout = ImageLayout::Stretch;
                    btnUpload->Text = "";

                    // Remove borders
                    btnUpload->FlatStyle = FlatStyle::Flat;
                    btnUpload->FlatAppearance->BorderSize = 0;
                    btnUpload->BackColor = Color::Transparent;
                }

                // Load Play button image
                String^ playImagePath = Path::Combine(basePath, "Images\\btnplay.jpg");
                if (File::Exists(playImagePath))
                {
                    btnPlay->BackgroundImage = Image::FromFile(playImagePath);
                    btnPlay->BackgroundImageLayout = ImageLayout::Stretch;
                    btnPlay->Text = "";

                    // Remove borders
                    btnPlay->FlatStyle = FlatStyle::Flat;
                    btnPlay->FlatAppearance->BorderSize = 0;
                    btnPlay->BackColor = Color::Transparent;
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Error loading button images: " + ex->Message);
            }
        }

        // Load background image from Images folder
        void LoadBackgroundImage()
        {
            try
            {
                String^ imagePath = Path::Combine(Application::StartupPath, "Images\\background.jpg");

                if (File::Exists(imagePath))
                {
                    pictureBoxBackground->Image = Image::FromFile(imagePath);
                    pictureBoxBackground->SendToBack();
                }
                else
                {
                    MessageBox::Show("Image not found at: " + imagePath);
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Error loading image: " + ex->Message);
            }
        }

        // Upload function
        System::Void btnUpload_Click(System::Object^ sender, System::EventArgs^ e)
        {
            OpenFileDialog^ ofd = gcnew OpenFileDialog();
            ofd->Filter = "Video Files|*.mp4;*.avi;*.mkv;*.mov";
            ofd->Multiselect = true;

            if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            {
                for each (String ^ fullPath in ofd->FileNames)
                {
                    String^ name = Path::GetFileName(fullPath);
                    videoList->addVideo(fullPath, name, listBox2);
                }
            }
        }

        // Delete function
        System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex == -1)
            {
                MessageBox::Show("Please select a video to delete.","Error", MessageBoxButtons::OK,MessageBoxIcon::Error);
                return;
            }

            videoList->removeVideo(listBox2);
        }

        // Save playlist to file - Call this function when you want to save
        void SavePlaylist()
        {
            SaveFileDialog^ sfd = gcnew SaveFileDialog();
            sfd->Filter = "Playlist Files (*.txt)|*.txt|All Files (*.*)|*.*";
            sfd->DefaultExt = "txt";
            sfd->FileName = "playlist.txt";

            if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            {
                videoList->saveToFile(sfd->FileName, listBox2);
                MessageBox::Show("Playlist saved successfully!", "Save Complete");
            }
        }

        // Load playlist from file - Call this function when you want to load
        void LoadPlaylist()
        {
            OpenFileDialog^ ofd = gcnew OpenFileDialog();
            ofd->Filter = "Playlist Files (*.txt)|*.txt|All Files (*.*)|*.*";

            if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            {
                // Clear current list before loading
                listBox2->Items->Clear();

                videoList->loadFromFile(ofd->FileName, listBox2);
                MessageBox::Show("Playlist loaded successfully!", "Load Complete");
            }
        }

        // Populate function - Refreshes the ListBox display
        void PopulateListBox()
        {
            videoList->populateTrackList(listBox2);
        }

        // Auto-save playlist when closing the application
        void AutoSavePlaylist()
        {
            try
            {
                String^ autoSavePath = Path::Combine(Application::StartupPath, "playlist_autosave.txt");
                videoList->saveToFile(autoSavePath, listBox2);
            }
            catch (Exception^ ex)
            {
                // Silent fail - don't show error when closing
                System::Diagnostics::Debug::WriteLine("Auto-save failed: " + ex->Message);
            }
        }

        // Auto-load playlist when starting the application
        void AutoLoadPlaylist()
        {
            try
            {
                String^ autoSavePath = Path::Combine(Application::StartupPath, "playlist_autosave.txt");

                if (File::Exists(autoSavePath))
                {
                    videoList->loadFromFile(autoSavePath, listBox2);
                }
            }
            catch (Exception^ ex)
            {
                // Silent fail - don't show error on startup
                System::Diagnostics::Debug::WriteLine("Auto-load failed: " + ex->Message);
            }
        }


        
    };
}