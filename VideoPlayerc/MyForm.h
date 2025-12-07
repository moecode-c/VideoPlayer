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
        System::Windows::Forms::Button^ btnPlay;
        System::Windows::Forms::Button^ btnSortAlpha;
        System::Windows::Forms::Button^ btnSortTime;
        System::Windows::Forms::Button^ btnClearAll;
        System::Windows::Forms::Button^ btnBackToMenu;     // NEW - Back button
        System::Windows::Forms::Panel^ videoPanel;         // NEW - Panel to hold media player
        System::Windows::Forms::ListBox^ listBox2;
        System::Windows::Forms::PictureBox^ pictureBoxBackground;
        AxWMPLib::AxWindowsMediaPlayer^ mediaPlayer;
        VideoList^ videoList;
        bool isPlaying;

    public:
        MainForm(void)
        {
            InitializeComponent();
            videoList = gcnew VideoList();
            isPlaying = false;
            LoadBackgroundImage();
            LoadButtonImages();
            PositionListBox();
            PositionButtons();
            PositionMediaPlayer();
            AutoLoadPlaylist();
        }

    protected:
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

        virtual void OnFormClosing(FormClosingEventArgs^ e) override
        {
            AutoSavePlaylist();
            Form::OnFormClosing(e);
        }

        virtual void OnResize(EventArgs^ e) override
        {
            Form::OnResize(e);
            PositionListBox();
            PositionButtons();
            PositionMediaPlayer();
        }

    private:
        System::ComponentModel::Container^ components;

        void InitializeComponent(void)
        {
            this->btnDelete = (gcnew System::Windows::Forms::Button());
            this->btnUpload = (gcnew System::Windows::Forms::Button());
            this->btnPlay = (gcnew System::Windows::Forms::Button());
            this->btnSortAlpha = (gcnew System::Windows::Forms::Button());
            this->btnSortTime = (gcnew System::Windows::Forms::Button());
            this->btnClearAll = (gcnew System::Windows::Forms::Button());
            this->btnBackToMenu = (gcnew System::Windows::Forms::Button());
            this->videoPanel = (gcnew System::Windows::Forms::Panel());
            this->listBox2 = (gcnew System::Windows::Forms::ListBox());
            this->pictureBoxBackground = (gcnew System::Windows::Forms::PictureBox());
            this->mediaPlayer = (gcnew AxWMPLib::AxWindowsMediaPlayer());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mediaPlayer))->BeginInit();
            this->videoPanel->SuspendLayout();
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
            // btnPlay
            // 
            this->btnPlay->Location = System::Drawing::Point(394, 185);
            this->btnPlay->Name = L"btnPlay";
            this->btnPlay->Size = System::Drawing::Size(61, 60);
            this->btnPlay->TabIndex = 4;
            this->btnPlay->Text = L"Play";
            this->btnPlay->UseVisualStyleBackColor = true;
            this->btnPlay->Click += gcnew System::EventHandler(this, &MainForm::btnPlay_Click);
            // 
            // btnSortAlpha
            // 
            this->btnSortAlpha->Location = System::Drawing::Point(500, 112);
            this->btnSortAlpha->Name = L"btnSortAlpha";
            this->btnSortAlpha->Size = System::Drawing::Size(100, 40);
            this->btnSortAlpha->TabIndex = 5;
            this->btnSortAlpha->Text = L"Sort A-Z";
            this->btnSortAlpha->UseVisualStyleBackColor = true;
            this->btnSortAlpha->Click += gcnew System::EventHandler(this, &MainForm::btnSortAlpha_Click);
            // 
            // btnSortTime
            // 
            this->btnSortTime->Location = System::Drawing::Point(500, 162);
            this->btnSortTime->Name = L"btnSortTime";
            this->btnSortTime->Size = System::Drawing::Size(100, 40);
            this->btnSortTime->TabIndex = 6;
            this->btnSortTime->Text = L"Sort by Time";
            this->btnSortTime->UseVisualStyleBackColor = true;
            this->btnSortTime->Click += gcnew System::EventHandler(this, &MainForm::btnSortTime_Click);
            // 
            // btnClearAll
            // 
            this->btnClearAll->Location = System::Drawing::Point(500, 212);
            this->btnClearAll->Name = L"btnClearAll";
            this->btnClearAll->Size = System::Drawing::Size(100, 40);
            this->btnClearAll->TabIndex = 7;
            this->btnClearAll->Text = L"Clear All";
            this->btnClearAll->UseVisualStyleBackColor = true;
            this->btnClearAll->Click += gcnew System::EventHandler(this, &MainForm::btnClearAll_Click);
            // 
            // btnBackToMenu
            // 
            this->btnBackToMenu->BackColor = System::Drawing::Color::FromArgb(64, 64, 64);
            this->btnBackToMenu->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBackToMenu->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnBackToMenu->ForeColor = System::Drawing::Color::White;
            this->btnBackToMenu->Location = System::Drawing::Point(10, 10);
            this->btnBackToMenu->Name = L"btnBackToMenu";
            this->btnBackToMenu->Size = System::Drawing::Size(120, 40);
            this->btnBackToMenu->TabIndex = 8;
            this->btnBackToMenu->Text = L"← Back";
            this->btnBackToMenu->UseVisualStyleBackColor = false;
            this->btnBackToMenu->Click += gcnew System::EventHandler(this, &MainForm::btnBackToMenu_Click);
            // 
            // videoPanel
            // 
            this->videoPanel->BackColor = System::Drawing::Color::Black;
            this->videoPanel->Controls->Add(this->mediaPlayer);
            this->videoPanel->Controls->Add(this->btnBackToMenu);
            this->videoPanel->Location = System::Drawing::Point(50, 50);
            this->videoPanel->Name = L"videoPanel";
            this->videoPanel->Size = System::Drawing::Size(640, 380);
            this->videoPanel->TabIndex = 9;
            this->videoPanel->Visible = false;
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
            this->listBox2->DoubleClick += gcnew System::EventHandler(this, &MainForm::listBox2_DoubleClick);
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
            // mediaPlayer
            // 
            this->mediaPlayer->Enabled = true;
            this->mediaPlayer->Location = System::Drawing::Point(0, 60);
            this->mediaPlayer->Name = L"mediaPlayer";
            this->mediaPlayer->Size = System::Drawing::Size(640, 320);
            this->mediaPlayer->TabIndex = 0;
            this->mediaPlayer->PlayStateChange += gcnew AxWMPLib::_WMPOCXEvents_PlayStateChangeEventHandler(this, &MainForm::mediaPlayer_PlayStateChange);
            // 
            // MainForm
            // 
            this->ClientSize = System::Drawing::Size(742, 437);
            this->Controls->Add(this->videoPanel);
            this->Controls->Add(this->btnClearAll);
            this->Controls->Add(this->btnSortTime);
            this->Controls->Add(this->btnSortAlpha);
            this->Controls->Add(this->btnPlay);
            this->Controls->Add(this->listBox2);
            this->Controls->Add(this->btnUpload);
            this->Controls->Add(this->btnDelete);
            this->Controls->Add(this->pictureBoxBackground);
            this->Name = L"MainForm";
            this->Text = L"Video Player";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mediaPlayer))->EndInit();
            this->videoPanel->ResumeLayout(false);
            this->ResumeLayout(false);

        }

        void PositionListBox()
        {
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

            btnDelete->Location = System::Drawing::Point(startLeft, buttonTop);
            btnDelete->Size = System::Drawing::Size(buttonWidth, buttonHeight);

            int playLeft = startLeft + buttonWidth + spacing;
            btnPlay->Location = System::Drawing::Point(playLeft, buttonTop);
            btnPlay->Size = System::Drawing::Size(buttonWidth, buttonHeight);

            int uploadLeft = playLeft + buttonWidth + spacing;
            btnUpload->Location = System::Drawing::Point(uploadLeft, buttonTop);
            btnUpload->Size = System::Drawing::Size(buttonWidth, buttonHeight);

            double newButtonWidthPercent = 13.0;
            double newButtonHeightPercent = 9.0;
            int newButtonWidth = (int)(this->ClientSize.Width * newButtonWidthPercent / 100.0);
            int newButtonHeight = (int)(this->ClientSize.Height * newButtonHeightPercent / 100.0);

            double newButtonLeftPercent = 67.0;
            int newButtonLeft = (int)(this->ClientSize.Width * newButtonLeftPercent / 100.0);

            double newButtonTopPercent = 24.5;
            int newButtonTop = (int)(this->ClientSize.Height * newButtonTopPercent / 100.0);

            int verticalSpacing = (int)(this->ClientSize.Height * 2.0 / 100.0);

            btnSortAlpha->Location = System::Drawing::Point(newButtonLeft, newButtonTop);
            btnSortAlpha->Size = System::Drawing::Size(newButtonWidth, newButtonHeight);

            btnSortTime->Location = System::Drawing::Point(newButtonLeft, newButtonTop + newButtonHeight + verticalSpacing);
            btnSortTime->Size = System::Drawing::Size(newButtonWidth, newButtonHeight);

            btnClearAll->Location = System::Drawing::Point(newButtonLeft, newButtonTop + (newButtonHeight + verticalSpacing) * 2);
            btnClearAll->Size = System::Drawing::Size(newButtonWidth, newButtonHeight);
        }

        void PositionMediaPlayer()
        {
            // Center the video panel
            int panelWidth = (int)(this->ClientSize.Width * 0.85);
            int panelHeight = (int)(this->ClientSize.Height * 0.85);
            int panelLeft = (this->ClientSize.Width - panelWidth) / 2;
            int panelTop = (this->ClientSize.Height - panelHeight) / 2;

            videoPanel->Location = System::Drawing::Point(panelLeft, panelTop);
            videoPanel->Size = System::Drawing::Size(panelWidth, panelHeight);

            // Position back button at top-left of panel
            btnBackToMenu->Location = System::Drawing::Point(10, 10);

            // Position media player below back button, filling rest of panel
            int playerTop = 60;
            mediaPlayer->Location = System::Drawing::Point(0, playerTop);
            mediaPlayer->Size = System::Drawing::Size(panelWidth, panelHeight - playerTop);
        }

        void LoadButtonImages()
        {
            try
            {
                String^ basePath = Application::StartupPath;

                String^ deleteImagePath = Path::Combine(basePath, "Images\\btndelete.jpg");
                if (File::Exists(deleteImagePath))
                {
                    btnDelete->BackgroundImage = Image::FromFile(deleteImagePath);
                    btnDelete->BackgroundImageLayout = ImageLayout::Stretch;
                    btnDelete->Text = "";
                    btnDelete->FlatStyle = FlatStyle::Flat;
                    btnDelete->FlatAppearance->BorderSize = 0;
                    btnDelete->BackColor = Color::Transparent;
                }

                String^ uploadImagePath = Path::Combine(basePath, "Images\\btnupload.jpg");
                if (File::Exists(uploadImagePath))
                {
                    btnUpload->BackgroundImage = Image::FromFile(uploadImagePath);
                    btnUpload->BackgroundImageLayout = ImageLayout::Stretch;
                    btnUpload->Text = "";
                    btnUpload->FlatStyle = FlatStyle::Flat;
                    btnUpload->FlatAppearance->BorderSize = 0;
                    btnUpload->BackColor = Color::Transparent;
                }

                String^ playImagePath = Path::Combine(basePath, "Images\\btnplay.jpg");
                if (File::Exists(playImagePath))
                {
                    btnPlay->BackgroundImage = Image::FromFile(playImagePath);
                    btnPlay->BackgroundImageLayout = ImageLayout::Stretch;
                    btnPlay->Text = "";
                    btnPlay->FlatStyle = FlatStyle::Flat;
                    btnPlay->FlatAppearance->BorderSize = 0;
                    btnPlay->BackColor = Color::Transparent;
                }

                btnSortAlpha->FlatStyle = FlatStyle::Flat;
                btnSortAlpha->FlatAppearance->BorderSize = 0;

                btnSortTime->FlatStyle = FlatStyle::Flat;
                btnSortTime->FlatAppearance->BorderSize = 0;

                btnClearAll->FlatStyle = FlatStyle::Flat;
                btnClearAll->FlatAppearance->BorderSize = 0;
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Error loading button images: " + ex->Message);
            }
        }

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

        // Show video panel (hide menu)
        void ShowVideoPanel()
        {
            videoPanel->Visible = true;
            videoPanel->BringToFront();

            // Hide menu elements
            listBox2->Visible = false;
            btnDelete->Visible = false;
            btnUpload->Visible = false;
            btnPlay->Visible = false;
            btnSortAlpha->Visible = false;
            btnSortTime->Visible = false;
            btnClearAll->Visible = false;
        }

        // Show menu (hide video panel)
        void ShowMenu()
        {
            videoPanel->Visible = false;

            // Show menu elements
            listBox2->Visible = true;
            btnDelete->Visible = true;
            btnUpload->Visible = true;
            btnPlay->Visible = true;
            btnSortAlpha->Visible = true;
            btnSortTime->Visible = true;
            btnClearAll->Visible = true;
        }

        // Back to menu button
        System::Void btnBackToMenu_Click(System::Object^ sender, System::EventArgs^ e)
        {
            // Stop video
            if (isPlaying)
            {
                mediaPlayer->Ctlcontrols->stop();
                isPlaying = false;
            }

            // Show menu, hide video
            ShowMenu();
        }

        // Play button
        System::Void btnPlay_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex >= 0)
            {
                videoList->setCurrentNode(listBox2->SelectedIndex);
            }

            String^ videoPath = videoList->getCurrentNodePath();

            if (videoPath != nullptr && File::Exists(videoPath))
            {
                mediaPlayer->URL = videoPath;
                ShowVideoPanel();
                mediaPlayer->Ctlcontrols->play();
                isPlaying = true;
            }
            else
            {
                MessageBox::Show("Please select a video to play.", "No Video Selected",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        // Double-click on listbox to play video
        System::Void listBox2_DoubleClick(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex >= 0)
            {
                btnPlay_Click(sender, e);
            }
        }

        // Handle when video ends - auto-play next
        System::Void mediaPlayer_PlayStateChange(System::Object^ sender, AxWMPLib::_WMPOCXEvents_PlayStateChangeEvent^ e)
        {
            if (e->newState == 8) // MediaEnded
            {
                String^ nextPath = videoList->nextVideo();
                if (nextPath != nullptr)
                {
                    mediaPlayer->URL = nextPath;
                    mediaPlayer->Ctlcontrols->play();
                }
                else
                {
                    isPlaying = false;
                    ShowMenu(); // Auto return to menu when playlist ends
                }
            }
            else if (e->newState == 1) // Stopped
            {
                isPlaying = false;
            }
            else if (e->newState == 3) // Playing
            {
                isPlaying = true;
            }
        }

        System::Void btnUpload_Click(System::Object^ sender, System::EventArgs^ e)
        {
            OpenFileDialog^ ofd = gcnew OpenFileDialog();
            ofd->Filter = "Video Files|*.mp4;*.avi;*.mkv;*.mov;*.wmv;*.flv";
            ofd->Multiselect = true;

            if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            {
                for each(String ^ fullPath in ofd->FileNames)
                {
                    String^ name = Path::GetFileName(fullPath);
                    videoList->addVideo(fullPath, name, listBox2);
                }

                PopulateListBox();
                listBox2->Refresh();
            }
        }

        System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex == -1)
            {
                MessageBox::Show("Please select a video to delete.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            videoList->removeVideo(listBox2);
        }

        System::Void btnSortAlpha_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->sortAlphabetically(listBox2);
        }

        System::Void btnSortTime_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->sortByTime(listBox2);
        }

        System::Void btnClearAll_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->clearAll(listBox2);
        }

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

        void LoadPlaylist()
        {
            OpenFileDialog^ ofd = gcnew OpenFileDialog();
            ofd->Filter = "Playlist Files (*.txt)|*.txt|All Files (*.*)|*.*";

            if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            {
                listBox2->Items->Clear();
                videoList->loadFromFile(ofd->FileName, listBox2);
                MessageBox::Show("Playlist loaded successfully!", "Load Complete");
            }
        }

        void PopulateListBox()
        {
            videoList->populateTrackList(listBox2);
        }

        void AutoSavePlaylist()
        {
            try
            {
                String^ autoSavePath = Path::Combine(Application::StartupPath, "playlist_autosave.txt");
                videoList->saveToFile(autoSavePath, listBox2);
            }
            catch (Exception^ ex)
            {
                System::Diagnostics::Debug::WriteLine("Auto-save failed: " + ex->Message);
            }
        }

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
                System::Diagnostics::Debug::WriteLine("Auto-load failed: " + ex->Message);
            }
        }
    };
}