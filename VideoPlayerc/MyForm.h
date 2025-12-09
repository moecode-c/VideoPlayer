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
        bool isDraggingSlider;

        // Extra controls (added features)
        System::Windows::Forms::Panel^ controlPanel;
        System::Windows::Forms::Button^ playButton;
        System::Windows::Forms::Button^ pauseButton;
        System::Windows::Forms::Button^ nextButton;
        System::Windows::Forms::Button^ previousButton;
        System::Windows::Forms::Button^ shuffleButton;
        System::Windows::Forms::Button^ fullScreenButton;
        System::Windows::Forms::Button^ loopButton; // NEW - Loop toggle button
        System::Windows::Forms::Button^ skipForwardButton; // NEW - +5s
        System::Windows::Forms::Button^ skipBackwardButton; // NEW - -5s

        System::Windows::Forms::Button^ resetButton;
        System::Windows::Forms::TrackBar^ positionTrackBar;
        System::Windows::Forms::TrackBar^ volumeTrackBar;
        System::Windows::Forms::Label^ timeLabel;
        System::Windows::Forms::ComboBox^ speedComboBox;
        System::Windows::Forms::Label^ speedLabel;

        System::Windows::Forms::Label^ playlistInfoLabel;
        System::Windows::Forms::Label^ volumeLabel;
        System::Windows::Forms::Timer^ timer;
        bool isFullscreen;
        enum class LoopMode { Off, One, All };
        LoopMode loopMode; // NEW - loop mode state
        // Store original positions to restore after exiting fullscreen
        System::Collections::Generic::Dictionary<String^, System::Drawing::Point>^ originalPositions;

    public:
        MainForm(void)
        {
            InitializeComponent();
            // Ensure AxWMPLib control is fully initialized before changing ActiveX properties
            this->mediaPlayer->uiMode = "none";
            videoList = gcnew VideoList();
            isPlaying = false;
            isDraggingSlider = false;
            LoadBackgroundImage();
            LoadButtonImages();
            PositionListBox();
            PositionButtons();
            PositionMediaPlayer();
            AutoLoadPlaylist();
            isFullscreen = false;
            loopMode = LoopMode::Off;
            this->CaptureOriginalControlPositions();
        }

    protected:
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }

        // Implementations are defined after the class
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
    private: System::ComponentModel::IContainer^ components;
    protected:

    private:
        // Helpers to manage control positions across fullscreen toggles
        void CaptureOriginalControlPositions();
        void RestoreOriginalControlPositions();
        void AddPos(System::String^ key, System::Windows::Forms::Control^ c);
        void SetPos(System::String^ key, System::Windows::Forms::Control^ c);


        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
            this->btnDelete = (gcnew System::Windows::Forms::Button());
            this->btnUpload = (gcnew System::Windows::Forms::Button());
            this->btnPlay = (gcnew System::Windows::Forms::Button());
            this->btnSortAlpha = (gcnew System::Windows::Forms::Button());
            this->btnSortTime = (gcnew System::Windows::Forms::Button());
            this->btnClearAll = (gcnew System::Windows::Forms::Button());
            this->btnBackToMenu = (gcnew System::Windows::Forms::Button());
            this->videoPanel = (gcnew System::Windows::Forms::Panel());
            this->mediaPlayer = (gcnew AxWMPLib::AxWindowsMediaPlayer());
            this->controlPanel = (gcnew System::Windows::Forms::Panel());
            this->positionTrackBar = (gcnew System::Windows::Forms::TrackBar());
            this->timeLabel = (gcnew System::Windows::Forms::Label());
            this->playButton = (gcnew System::Windows::Forms::Button());
            this->pauseButton = (gcnew System::Windows::Forms::Button());
            this->previousButton = (gcnew System::Windows::Forms::Button());
            this->nextButton = (gcnew System::Windows::Forms::Button());
            this->resetButton = (gcnew System::Windows::Forms::Button());
            this->shuffleButton = (gcnew System::Windows::Forms::Button());
            this->fullScreenButton = (gcnew System::Windows::Forms::Button());
            this->loopButton = (gcnew System::Windows::Forms::Button());
            this->skipForwardButton = (gcnew System::Windows::Forms::Button());
            this->skipBackwardButton = (gcnew System::Windows::Forms::Button());
            this->volumeLabel = (gcnew System::Windows::Forms::Label());
            this->volumeTrackBar = (gcnew System::Windows::Forms::TrackBar());
            this->speedLabel = (gcnew System::Windows::Forms::Label());
            this->speedComboBox = (gcnew System::Windows::Forms::ComboBox());
            this->playlistInfoLabel = (gcnew System::Windows::Forms::Label());
            this->timer = (gcnew System::Windows::Forms::Timer(this->components));
            this->listBox2 = (gcnew System::Windows::Forms::ListBox());
            this->pictureBoxBackground = (gcnew System::Windows::Forms::PictureBox());
            this->videoPanel->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mediaPlayer))->BeginInit();
            this->controlPanel->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->positionTrackBar))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeTrackBar))->BeginInit();
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
            this->btnBackToMenu->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
                static_cast<System::Int32>(static_cast<System::Byte>(64)));
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
            this->videoPanel->Controls->Add(this->controlPanel);
            this->videoPanel->Controls->Add(this->btnBackToMenu);
            this->videoPanel->Location = System::Drawing::Point(50, 16);
            this->videoPanel->Name = L"videoPanel";
            this->videoPanel->Size = System::Drawing::Size(640, 380);
            this->videoPanel->TabIndex = 9;
            this->videoPanel->Visible = false;
            // 
            // mediaPlayer
            // 
            this->mediaPlayer->Enabled = true;
            this->mediaPlayer->Location = System::Drawing::Point(0, -49);
            this->mediaPlayer->Name = L"mediaPlayer";
            this->mediaPlayer->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"mediaPlayer.OcxState")));
            this->mediaPlayer->Size = System::Drawing::Size(640, 438);
            this->mediaPlayer->TabIndex = 0;
            this->mediaPlayer->PlayStateChange += gcnew AxWMPLib::_WMPOCXEvents_PlayStateChangeEventHandler(this, &MainForm::mediaPlayer_PlayStateChange);
            // 
            // controlPanel
            // 
            // Disable AutoSize to prevent content-driven relayout when docked at bottom
            this->controlPanel->AutoSize = false;
            this->controlPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(73)),
                static_cast<System::Int32>(static_cast<System::Byte>(94)));
            this->controlPanel->Controls->Add(this->positionTrackBar);
            this->controlPanel->Controls->Add(this->timeLabel);
            this->controlPanel->Controls->Add(this->playButton);
            this->controlPanel->Controls->Add(this->pauseButton);
            this->controlPanel->Controls->Add(this->previousButton);
            this->controlPanel->Controls->Add(this->nextButton);
            this->controlPanel->Controls->Add(this->resetButton);
            this->controlPanel->Controls->Add(this->shuffleButton);
            this->controlPanel->Controls->Add(this->fullScreenButton);
            this->controlPanel->Controls->Add(this->loopButton);
            this->controlPanel->Controls->Add(this->skipForwardButton);
            this->controlPanel->Controls->Add(this->skipBackwardButton);
            this->controlPanel->Controls->Add(this->volumeLabel);
            this->controlPanel->Controls->Add(this->volumeTrackBar);
            this->controlPanel->Controls->Add(this->speedLabel);
            this->controlPanel->Controls->Add(this->speedComboBox);
            this->controlPanel->Cursor = System::Windows::Forms::Cursors::Hand;
            this->controlPanel->Location = System::Drawing::Point(0, 349);
            this->controlPanel->Name = L"controlPanel";
            this->controlPanel->Size = System::Drawing::Size(640, 146);
            this->controlPanel->TabIndex = 10;
            this->controlPanel->Visible = false;
            // Keep centered layout when the panel resizes (e.g., fullscreen)
            this->controlPanel->Resize += gcnew System::EventHandler(this, &MainForm::ControlPanel_Resize);
            // 
            // positionTrackBar
            // 
            this->positionTrackBar->AutoSize = false;
            this->positionTrackBar->LargeChange = 4;
            this->positionTrackBar->Location = System::Drawing::Point(10, 2);
            this->positionTrackBar->Margin = System::Windows::Forms::Padding(2);
            this->positionTrackBar->Maximum = 100;
            this->positionTrackBar->Name = L"positionTrackBar";
            this->positionTrackBar->Size = System::Drawing::Size(593, 27);
            this->positionTrackBar->TabIndex = 0;
            this->positionTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
            this->positionTrackBar->Scroll += gcnew System::EventHandler(this, &MainForm::PositionTrackBar_Scroll);
            this->positionTrackBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::PositionTrackBar_MouseDown);
            this->positionTrackBar->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::PositionTrackBar_MouseUp);
            // 
            // timeLabel
            // 
            this->timeLabel->ForeColor = System::Drawing::Color::White;
            this->timeLabel->Location = System::Drawing::Point(100, 31);
            this->timeLabel->Name = L"timeLabel";
            this->timeLabel->Size = System::Drawing::Size(450, 17);
            this->timeLabel->TabIndex = 1;
            this->timeLabel->Text = L"00:00 / 00:00";
            this->timeLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            // 
            // playButton
            // 
            this->playButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(46)), static_cast<System::Int32>(static_cast<System::Byte>(204)),
                static_cast<System::Int32>(static_cast<System::Byte>(113)));
            this->playButton->FlatAppearance->BorderSize = 0;
            this->playButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->playButton->ForeColor = System::Drawing::Color::White;
            this->playButton->Location = System::Drawing::Point(291, 62);
            this->playButton->Name = L"playButton";
            this->playButton->Size = System::Drawing::Size(60, 35);
            this->playButton->TabIndex = 2;
            this->playButton->Text = L"Play";
            this->playButton->UseVisualStyleBackColor = false;
            this->playButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->playButton->Click += gcnew System::EventHandler(this, &MainForm::PlayButton_Click);
            // 
            // pauseButton
            // 
            this->pauseButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(231)), static_cast<System::Int32>(static_cast<System::Byte>(76)),
                static_cast<System::Int32>(static_cast<System::Byte>(60)));
            this->pauseButton->FlatAppearance->BorderSize = 0;
            this->pauseButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->pauseButton->ForeColor = System::Drawing::Color::White;
            this->pauseButton->Location = System::Drawing::Point(291, 62);
            this->pauseButton->Name = L"pauseButton";
            this->pauseButton->Size = System::Drawing::Size(60, 35);
            this->pauseButton->TabIndex = 3;
            this->pauseButton->Text = L"Pause";
            this->pauseButton->UseVisualStyleBackColor = false;
            this->pauseButton->Visible = false;
            this->pauseButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->pauseButton->Click += gcnew System::EventHandler(this, &MainForm::PauseButton_Click);
            // 
            // previousButton
            // 
            this->previousButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(152)),
                static_cast<System::Int32>(static_cast<System::Byte>(219)));
            this->previousButton->FlatAppearance->BorderSize = 0;
            this->previousButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->previousButton->ForeColor = System::Drawing::Color::White;
            this->previousButton->Location = System::Drawing::Point(162, 65);
            this->previousButton->Name = L"previousButton";
            this->previousButton->Size = System::Drawing::Size(45, 35);
            this->previousButton->TabIndex = 4;
            this->previousButton->Text = L"Prev";
            this->previousButton->UseVisualStyleBackColor = false;
            this->previousButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->previousButton->Click += gcnew System::EventHandler(this, &MainForm::PreviousButton_Click);
            // 
            // nextButton
            // 
            this->nextButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(152)),
                static_cast<System::Int32>(static_cast<System::Byte>(219)));
            this->nextButton->FlatAppearance->BorderSize = 0;
            this->nextButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->nextButton->ForeColor = System::Drawing::Color::White;
            this->nextButton->Location = System::Drawing::Point(450, 62);
            this->nextButton->Name = L"nextButton";
            this->nextButton->Size = System::Drawing::Size(45, 35);
            this->nextButton->TabIndex = 5;
            this->nextButton->Text = L"Next";
            this->nextButton->UseVisualStyleBackColor = false;
            this->nextButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->nextButton->Click += gcnew System::EventHandler(this, &MainForm::NextButton_Click);
            // 
            // resetButton
            // 
            this->resetButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(241)), static_cast<System::Int32>(static_cast<System::Byte>(196)),
                static_cast<System::Int32>(static_cast<System::Byte>(15)));
            this->resetButton->FlatAppearance->BorderSize = 0;
            this->resetButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->resetButton->ForeColor = System::Drawing::Color::White;
            this->resetButton->Location = System::Drawing::Point(527, 62);
            this->resetButton->Name = L"resetButton";
            this->resetButton->Size = System::Drawing::Size(95, 25);
            this->resetButton->TabIndex = 6;
            this->resetButton->Text = L"Reset";
            this->resetButton->UseVisualStyleBackColor = false;
            this->resetButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->resetButton->Click += gcnew System::EventHandler(this, &MainForm::ResetButton_Click);
            // 
            // shuffleButton
            // 
            this->shuffleButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(155)), static_cast<System::Int32>(static_cast<System::Byte>(89)),
                static_cast<System::Int32>(static_cast<System::Byte>(182)));
            this->shuffleButton->FlatAppearance->BorderSize = 0;
            this->shuffleButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->shuffleButton->ForeColor = System::Drawing::Color::White;
            this->shuffleButton->Location = System::Drawing::Point(527, 102);
            this->shuffleButton->Name = L"shuffleButton";
            this->shuffleButton->Size = System::Drawing::Size(95, 25);
            this->shuffleButton->TabIndex = 7;
            this->shuffleButton->Text = L"Shuffle";
            this->shuffleButton->UseVisualStyleBackColor = false;
            this->shuffleButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->shuffleButton->Click += gcnew System::EventHandler(this, &MainForm::ShuffleButton_Click);
            // 
            // fullScreenButton
            // 
            this->fullScreenButton->BackColor = System::Drawing::Color::DimGray;
            this->fullScreenButton->FlatAppearance->BorderSize = 0;
            this->fullScreenButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->fullScreenButton->ForeColor = System::Drawing::Color::White;
            this->fullScreenButton->Location = System::Drawing::Point(426, 103);
            this->fullScreenButton->Name = L"fullScreenButton";
            this->fullScreenButton->Size = System::Drawing::Size(95, 25);
            this->fullScreenButton->TabIndex = 13;
            this->fullScreenButton->Text = L"Full Screen";
            this->fullScreenButton->UseVisualStyleBackColor = false;
            this->fullScreenButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->fullScreenButton->Click += gcnew System::EventHandler(this, &MainForm::FullScreenButton_Click);
            // 
            // loopButton
            // 
            this->loopButton->BackColor = System::Drawing::Color::Gray;
            this->loopButton->FlatAppearance->BorderSize = 0;
            this->loopButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->loopButton->ForeColor = System::Drawing::Color::White;
            this->loopButton->Location = System::Drawing::Point(10, 103);
            this->loopButton->Name = L"loopButton";
            this->loopButton->Size = System::Drawing::Size(95, 25);
            this->loopButton->TabIndex = 14;
            this->loopButton->Text = L"Loop: Off";
            this->loopButton->UseVisualStyleBackColor = false;
            this->loopButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->loopButton->Click += gcnew System::EventHandler(this, &MainForm::LoopButton_Click);
            // 
            // skipForwardButton
            // 
            this->skipForwardButton->BackColor = System::Drawing::Color::DarkSlateGray;
            this->skipForwardButton->FlatAppearance->BorderSize = 0;
            this->skipForwardButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->skipForwardButton->ForeColor = System::Drawing::Color::White;
            this->skipForwardButton->Location = System::Drawing::Point(369, 62);
            this->skipForwardButton->Name = L"skipForwardButton";
            this->skipForwardButton->Size = System::Drawing::Size(60, 35);
            this->skipForwardButton->TabIndex = 15;
            this->skipForwardButton->Text = L"+5s";
            this->skipForwardButton->UseVisualStyleBackColor = false;
            this->skipForwardButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->skipForwardButton->Click += gcnew System::EventHandler(this, &MainForm::SkipForwardButton_Click);
            // 
            // skipBackwardButton
            // 
            this->skipBackwardButton->BackColor = System::Drawing::Color::DarkSlateGray;
            this->skipBackwardButton->FlatAppearance->BorderSize = 0;
            this->skipBackwardButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->skipBackwardButton->ForeColor = System::Drawing::Color::White;
            this->skipBackwardButton->Location = System::Drawing::Point(213, 65);
            this->skipBackwardButton->Name = L"skipBackwardButton";
            this->skipBackwardButton->Size = System::Drawing::Size(60, 35);
            this->skipBackwardButton->TabIndex = 16;
            this->skipBackwardButton->Text = L"-5s";
            this->skipBackwardButton->UseVisualStyleBackColor = false;
            this->skipBackwardButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->skipBackwardButton->Click += gcnew System::EventHandler(this, &MainForm::SkipBackwardButton_Click);
            // 
            // volumeLabel
            // 
            this->volumeLabel->ForeColor = System::Drawing::Color::White;
            this->volumeLabel->Location = System::Drawing::Point(7, 67);
            this->volumeLabel->Name = L"volumeLabel";
            this->volumeLabel->Size = System::Drawing::Size(50, 20);
            this->volumeLabel->TabIndex = 8;
            this->volumeLabel->Text = L"Vol:";
            // 
            // volumeTrackBar
            // 
            this->volumeTrackBar->Location = System::Drawing::Point(29, 63);
            this->volumeTrackBar->Maximum = 100;
            this->volumeTrackBar->Name = L"volumeTrackBar";
            this->volumeTrackBar->Size = System::Drawing::Size(127, 56);
            this->volumeTrackBar->TabIndex = 9;
            this->volumeTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
            this->volumeTrackBar->Value = 50;
            this->volumeTrackBar->Scroll += gcnew System::EventHandler(this, &MainForm::VolumeTrackBar_Scroll);
            // 
            // speedLabel
            // 
            this->speedLabel->ForeColor = System::Drawing::Color::White;
            this->speedLabel->Location = System::Drawing::Point(182, 103);
            this->speedLabel->Name = L"speedLabel";
            this->speedLabel->Size = System::Drawing::Size(50, 20);
            this->speedLabel->TabIndex = 11;
            this->speedLabel->Text = L"Speed:";
            // 
            // speedComboBox
            // 
            this->speedComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->speedComboBox->FormattingEnabled = true;
            this->speedComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(5) { L"0.5x", L"1.0x", L"1.25x", L"1.5x", L"2.0x" });
            this->speedComboBox->Location = System::Drawing::Point(249, 103);
            this->speedComboBox->Name = L"speedComboBox";
            this->speedComboBox->Size = System::Drawing::Size(80, 24);
            this->speedComboBox->TabIndex = 12;
            this->speedComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::SpeedComboBox_SelectedIndexChanged);
            // 
            // playlistInfoLabel
            // 
            this->playlistInfoLabel->Location = System::Drawing::Point(0, 0);
            this->playlistInfoLabel->Name = L"playlistInfoLabel";
            this->playlistInfoLabel->Size = System::Drawing::Size(100, 23);
            this->playlistInfoLabel->TabIndex = 0;
            // 
            // timer
            // 
            this->timer->Tick += gcnew System::EventHandler(this, &MainForm::Timer_Tick);
            // 
            // listBox2
            // 
            this->listBox2->BackColor = System::Drawing::SystemColors::InactiveCaption;
            this->listBox2->FormattingEnabled = true;
            this->listBox2->ItemHeight = 16;
            this->listBox2->Location = System::Drawing::Point(137, 112);
            this->listBox2->Name = L"listBox2";
            this->listBox2->Size = System::Drawing::Size(177, 212);
            this->listBox2->TabIndex = 3;
            this->listBox2->DoubleClick += gcnew System::EventHandler(this, &MainForm::listBox2_DoubleClick);
            // 
            // pictureBoxBackground
            // 
            this->pictureBoxBackground->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxBackground->Location = System::Drawing::Point(0, 0);
            this->pictureBoxBackground->Name = L"pictureBoxBackground";
            this->pictureBoxBackground->Size = System::Drawing::Size(742, 574);
            this->pictureBoxBackground->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->pictureBoxBackground->TabIndex = 0;
            this->pictureBoxBackground->TabStop = false;
            // 
            // MainForm
            // 
            this->ClientSize = System::Drawing::Size(742, 574);
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
            this->videoPanel->ResumeLayout(false);
            this->videoPanel->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mediaPlayer))->EndInit();
            this->controlPanel->ResumeLayout(false);
            this->controlPanel->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->positionTrackBar))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeTrackBar))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->EndInit();
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
            // With docking, mediaPlayer fills the remaining space above the bottom controlPanel
            // Keep existing button layout in normal mode
        }

        // Center the playback controls horizontally within controlPanel
        void ArrangeControlsCentered()
        {
            if (controlPanel == nullptr) return;
            if (!isFullscreen) return; // Only center in fullscreen to preserve original layout in normal mode
            // Define the row of controls to center (left-to-right order)
            cli::array<Control^>^ row = gcnew cli::array<Control^>(8) {
                previousButton,
                skipBackwardButton,
                playButton->Visible ? (Control^)playButton : (Control^)pauseButton,
                skipForwardButton,
                nextButton,
                resetButton,
                fullScreenButton,
                shuffleButton
            };

            // Compute total width including spacing
            int spacing = 12; // px between controls
            int totalWidth = 0;
            int count = 0;
            for each (Control ^ c in row)
            {
                if (c == nullptr) continue;
                totalWidth += c->Width;
                count++;
            }
            totalWidth += Math::Max(0, (count - 1) * spacing);

            int startX = (controlPanel->ClientSize.Width - totalWidth) / 2;
            int y = 62; // baseline row Y

            // Place controls
            for each (Control ^ c in row)
            {
                if (c == nullptr) continue;
                c->Location = System::Drawing::Point(startX, y);
                startX += c->Width + spacing;
            }

            // Keep positionTrackBar and timeLabel centered
            if (positionTrackBar != nullptr)
            {
                positionTrackBar->Width = controlPanel->ClientSize.Width - 20;
                positionTrackBar->Location = System::Drawing::Point(10, positionTrackBar->Location.Y);
            }
            if (timeLabel != nullptr)
            {
                timeLabel->Width = controlPanel->ClientSize.Width - 200;
                timeLabel->Location = System::Drawing::Point((controlPanel->ClientSize.Width - timeLabel->Width)/2, timeLabel->Location.Y);
            }

            // Keep left-side volume, loop, and speed near edges consistently
            if (volumeLabel != nullptr && volumeTrackBar != nullptr)
            {
                volumeLabel->Location = System::Drawing::Point(10, volumeLabel->Location.Y);
                volumeTrackBar->Location = System::Drawing::Point(volumeLabel->Right + 12, volumeTrackBar->Location.Y);
            }
            if (loopButton != nullptr)
            {
                loopButton->Location = System::Drawing::Point(10, loopButton->Location.Y);
            }
            if (speedLabel != nullptr && speedComboBox != nullptr)
            {
                int rightPadding = 10;
                speedComboBox->Location = System::Drawing::Point(controlPanel->ClientSize.Width - speedComboBox->Width - rightPadding, speedComboBox->Location.Y);
                speedLabel->Location = System::Drawing::Point(speedComboBox->Left - speedLabel->Width - 8, speedLabel->Location.Y);
            }
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
            controlPanel->Visible = true;
            controlPanel->BringToFront();

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
            controlPanel->Visible = false;

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
                // Apply current speed selection
                ApplySelectedPlaybackRate();
                isPlaying = true;
                controlPanel->Visible = true;
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
                if (loopMode == LoopMode::One && mediaPlayer->currentMedia != nullptr)
                {
                    mediaPlayer->Ctlcontrols->currentPosition = 0;
                    mediaPlayer->Ctlcontrols->play();
                    ApplySelectedPlaybackRate();
                    isPlaying = true;
                }
                else
                {
                    String^ nextPath = videoList->nextVideo();
                    if (nextPath != nullptr)
                    {
                        // advance selection for UI
                        int idx = listBox2->SelectedIndex;
                        if (idx >= 0 && idx < listBox2->Items->Count - 1) listBox2->SelectedIndex = idx + 1;
                        mediaPlayer->URL = nextPath;
                        mediaPlayer->Ctlcontrols->play();
                        ApplySelectedPlaybackRate();
                        isPlaying = true;
                        controlPanel->Visible = true;
                    }
                    else
                    {
                        if (loopMode == LoopMode::All && listBox2->Items->Count > 0)
                        {
                            listBox2->SelectedIndex = 0;
                            videoList->setCurrentNode(0);
                            String^ path = videoList->getCurrentNodePath();
                            if (path != nullptr) {
                                mediaPlayer->URL = path;
                                mediaPlayer->Ctlcontrols->play();
                                ApplySelectedPlaybackRate();
                                isPlaying = true;
                                controlPanel->Visible = true;
                            }
                        }
                        else
                        {
                            isPlaying = false;
                        }
                    }
                }
            }
            else if (e->newState == 1) // Stopped
            {
                isPlaying = false;
                timer->Enabled = false;
            }
            else if (e->newState == 3) // Playing
            {
                isPlaying = true;
                controlPanel->Visible = true;
                // keep timer driving progress
                timer->Interval = 200;
                timer->Enabled = true;
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

        // Event Handlers
        void PlayButton_Click(Object^ sender, EventArgs^ e) {
            if (mediaPlayer->URL != nullptr && mediaPlayer->URL != "") {
                mediaPlayer->Ctlcontrols->play();
                ApplySelectedPlaybackRate();
                playButton->Visible = false;
                pauseButton->Visible = true;
                isPlaying = true;
                // Keep centered layout correct in fullscreen after toggling Play/Pause
                if (isFullscreen) ArrangeControlsCentered();
            }
        }

        void PauseButton_Click(Object^ sender, EventArgs^ e) {
            mediaPlayer->Ctlcontrols->pause();
            pauseButton->Visible = false;
            playButton->Visible = true;
            isPlaying = false;
            // Keep centered layout correct in fullscreen after toggling Play/Pause
            if (isFullscreen) ArrangeControlsCentered();
        }

        void NextButton_Click(Object^ sender, EventArgs^ e) {
            String^ nextPath = videoList->nextVideo();
            if (nextPath != nullptr) {
                mediaPlayer->URL = nextPath;
                mediaPlayer->Ctlcontrols->play();
                isPlaying = true;
            }
        }

        void SpeedComboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            ApplySelectedPlaybackRate();
        }

        void ApplySelectedPlaybackRate() {
            if (speedComboBox == nullptr) return;
            String^ sel = safe_cast<String^>(speedComboBox->SelectedItem);
            if (sel == nullptr) return;
            double rate = 1.0;
            if (sel->StartsWith(L"0.5")) rate = 0.5;
            else if (sel->StartsWith(L"1.0")) rate = 1.0;
            else if (sel->StartsWith(L"1.25")) rate = 1.25;
            else if (sel->StartsWith(L"1.5")) rate = 1.5;
            else if (sel->StartsWith(L"2.0")) rate = 2.0;
            mediaPlayer->settings->rate = rate;
        }

        void PreviousButton_Click(Object^ sender, EventArgs^ e) {
            int idx = listBox2->SelectedIndex;
            if (idx <= 0 && listBox2->Items->Count > 0) {
                idx = 0;
            } else if (idx > 0) {
                idx = idx - 1;
            } else {
                return;
            }

            listBox2->SelectedIndex = idx;
            videoList->setCurrentNode(idx);
            String^ path = videoList->getCurrentNodePath();
            if (path != nullptr) {
                mediaPlayer->URL = path;
                if (isPlaying) {
                    mediaPlayer->Ctlcontrols->play();
                }
            }
        }

        void ResetButton_Click(Object^ sender, EventArgs^ e) {
            if (listBox2->Items->Count == 0) return;
            listBox2->SelectedIndex = 0;
            videoList->setCurrentNode(0);
            String^ path = videoList->getCurrentNodePath();
            if (path != nullptr) {
                mediaPlayer->URL = path;
            }
            MessageBox::Show("Playlist reset to first video!", "Reset",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        void OpenButton_Click(Object^ sender, EventArgs^ e) {
            OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
            openFileDialog->Filter = "Video Files|*.mp4;*.avi;*.wmv;*.mkv;*.mov|All Files|*.*";
            openFileDialog->Title = "Select a Video File";
            openFileDialog->Multiselect = true;

            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                for each (String ^ file in openFileDialog->FileNames) {
                    String^ name = System::IO::Path::GetFileName(file);
                    videoList->addVideo(file, name, listBox2);
                }
                PopulateListBox();
            }
        }

        void PositionTrackBar_MouseDown(Object^ sender, MouseEventArgs^ e) {
            isDraggingSlider = true;
        }

        void PositionTrackBar_MouseUp(Object^ sender, MouseEventArgs^ e) {
            isDraggingSlider = false;
            if (mediaPlayer->currentMedia != nullptr) {
                double duration = mediaPlayer->currentMedia->duration;
                double newPosition = (positionTrackBar->Value / 100.0) * duration;
                mediaPlayer->Ctlcontrols->currentPosition = newPosition;
            }
        }

        void PositionTrackBar_Scroll(Object^ sender, EventArgs^ e) {
            if (!isDraggingSlider && mediaPlayer->currentMedia != nullptr) {
                double duration = mediaPlayer->currentMedia->duration;
                double newPosition = (positionTrackBar->Value / 100.0) * duration;
                mediaPlayer->Ctlcontrols->currentPosition = newPosition;
            }
        }

        void VolumeTrackBar_Scroll(Object^ sender, EventArgs^ e) {
            mediaPlayer->settings->volume = volumeTrackBar->Value;
        }

        void Timer_Tick(Object^ sender, EventArgs^ e) {
            if (mediaPlayer->currentMedia != nullptr && !isDraggingSlider) {
                double duration = mediaPlayer->currentMedia->duration;
                double position = mediaPlayer->Ctlcontrols->currentPosition;

                if (duration > 0) {
                    int percentage = (int)((position / duration) * 100);
                    positionTrackBar->Value = Math::Min(percentage, 100);
                }

                timeLabel->Text = FormatTime(position) + " / " + FormatTime(duration);

                // End handling is managed by PlayStateChange; no timer-driven next to avoid duplication
            }
        }

        void Button_MouseEnter(Object^ sender, EventArgs^ e) {
            Button^ btn = safe_cast<Button^>(sender);
            btn->BackColor = Color::FromArgb(
                Math::Min(255, btn->BackColor.R + 20),
                Math::Min(255, btn->BackColor.G + 20),
                Math::Min(255, btn->BackColor.B + 20)
            );
        }

        void SkipForwardButton_Click(Object^ sender, EventArgs^ e) {
            if (mediaPlayer->currentMedia == nullptr) return;
            double duration = mediaPlayer->currentMedia->duration;
            double pos = mediaPlayer->Ctlcontrols->currentPosition;
            double newPos = Math::Min(duration, pos + 5.0);
            mediaPlayer->Ctlcontrols->currentPosition = newPos;
        }

        void SkipBackwardButton_Click(Object^ sender, EventArgs^ e) {
            if (mediaPlayer->currentMedia == nullptr) return;
            double pos = mediaPlayer->Ctlcontrols->currentPosition;
            double newPos = Math::Max(0.0, pos - 5.0);
            mediaPlayer->Ctlcontrols->currentPosition = newPos;
        }

        void LoopButton_Click(Object^ sender, EventArgs^ e) {
            // Cycle through Off -> One -> All -> Off
            if (loopMode == LoopMode::Off) loopMode = LoopMode::One;
            else if (loopMode == LoopMode::One) loopMode = LoopMode::All;
            else loopMode = LoopMode::Off;

            if (loopMode == LoopMode::Off) {
                loopButton->Text = L"Loop: Off";
                loopButton->BackColor = System::Drawing::Color::Gray;
            } else if (loopMode == LoopMode::One) {
                loopButton->Text = L"Loop: One";
                loopButton->BackColor = System::Drawing::Color::FromArgb(76, 175, 80);
            } else {
                loopButton->Text = L"Loop: All";
                loopButton->BackColor = System::Drawing::Color::FromArgb(52, 152, 219);
            }
        }

        void Button_MouseLeave(Object^ sender, EventArgs^ e) {
            Button^ btn = safe_cast<Button^>(sender);
            if (btn == playButton || btn == pauseButton) {
                btn->BackColor = btn == playButton ?
                    Color::FromArgb(46, 204, 113) : Color::FromArgb(231, 76, 60);
            }
            else if (btn == nextButton || btn == previousButton) {
                btn->BackColor = Color::FromArgb(52, 152, 219);
            }
            else if (btn == resetButton) {
                btn->BackColor = Color::FromArgb(241, 196, 15);
            }
            else if (btn == shuffleButton) {
                btn->BackColor = Color::FromArgb(155, 89, 182);
            }
            else if (btn == loopButton) {
                if (loopMode == LoopMode::Off) btn->BackColor = Color::Gray;
                else if (loopMode == LoopMode::One) btn->BackColor = Color::FromArgb(76, 175, 80);
                else btn->BackColor = Color::FromArgb(52, 152, 219);
            }
            else if (btn == skipForwardButton || btn == skipBackwardButton) {
                btn->BackColor = Color::DarkSlateGray;
            }
        }

        void FullScreenButton_Click(Object^ sender, EventArgs^ e) {
            if (!isFullscreen) {
                this->WindowState = FormWindowState::Maximized;
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
                // Dock the video panel and media player to fill the window
                videoPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                mediaPlayer->Dock = System::Windows::Forms::DockStyle::Fill; // keep fill in both modes
                mediaPlayer->stretchToFit = true; // fill while preserving aspect
                // Keep controls at bottom
                controlPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
                videoPanel->BringToFront();
                controlPanel->BringToFront();
                // Auto-hide controls in fullscreen initially
                controlPanel->Visible = false;
                // React to mouse movement to show controls near bottom
                videoPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
                this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
                fullScreenButton->Text = L"Exit Full Screen";
                isFullscreen = true;
                ArrangeControlsCentered();
            }
            else {
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
                this->WindowState = FormWindowState::Normal;
                // Restore docking
                controlPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
                videoPanel->Dock = System::Windows::Forms::DockStyle::None;
                mediaPlayer->Dock = System::Windows::Forms::DockStyle::Fill; // keep fill inside panel
                mediaPlayer->stretchToFit = true;
                videoPanel->MouseMove -= gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
                this->MouseMove -= gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
                PositionMediaPlayer();
                fullScreenButton->Text = L"Full Screen";
                isFullscreen = false;
                controlPanel->Visible = true;
                controlPanel->BringToFront();
                // Restore original positions of controls after leaving fullscreen
                this->RestoreOriginalControlPositions();
                ArrangeControlsCentered();
            }
        }

        void VideoPanel_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            if (!isFullscreen) return;
            // Show controls when mouse is within 120px of bottom
            int threshold = 120;
            int distFromBottom = videoPanel->ClientSize.Height - e->Y;
            controlPanel->Visible = distFromBottom <= threshold;
        }

        // Keep controls centered whenever the bottom panel resizes while docked
        void ControlPanel_Resize(System::Object^ sender, System::EventArgs^ e) {
            ArrangeControlsCentered();
        }

        void ShuffleButton_Click(Object^ sender, EventArgs^ e) {
            // One-time shuffle: pick a random video and play it
            if (listBox2->Items->Count > 0) {
                System::Random^ rand = gcnew System::Random();
                int currentIdx = listBox2->SelectedIndex;
                int idx = rand->Next(0, listBox2->Items->Count);
                if (listBox2->Items->Count > 1 && idx == currentIdx) {
                    idx = (idx + 1) % listBox2->Items->Count;
                }
                listBox2->SelectedIndex = idx;
                videoList->setCurrentNode(idx);
                String^ path = videoList->getCurrentNodePath();
                if (path != nullptr) {
                    mediaPlayer->URL = path;
                    mediaPlayer->Ctlcontrols->play();
                    isPlaying = true;
                    controlPanel->Visible = true;
                    ShowVideoPanel();
                }
            }
        }

        String^ FormatTime(double seconds) {
            int min = (int)(seconds / 60);
            int sec = (int)seconds % 60;
            return String::Format(L"{0:D2}:{1:D2}", min, sec);
        }


};
}

// Helper implementations
namespace VideoPlayerc {
    void MainForm::CaptureOriginalControlPositions() {
        this->originalPositions = gcnew System::Collections::Generic::Dictionary<System::String^, System::Drawing::Point>();
        this->AddPos(L"previousButton", this->previousButton);
        this->AddPos(L"skipBackwardButton", this->skipBackwardButton);
        this->AddPos(L"playButton", this->playButton);
        this->AddPos(L"pauseButton", this->pauseButton);
        this->AddPos(L"skipForwardButton", this->skipForwardButton);
        this->AddPos(L"nextButton", this->nextButton);
        this->AddPos(L"resetButton", this->resetButton);
        this->AddPos(L"fullScreenButton", this->fullScreenButton);
        this->AddPos(L"shuffleButton", this->shuffleButton);
        this->AddPos(L"loopButton", this->loopButton);
        this->AddPos(L"volumeLabel", this->volumeLabel);
        this->AddPos(L"volumeTrackBar", this->volumeTrackBar);
        this->AddPos(L"speedLabel", this->speedLabel);
        this->AddPos(L"speedComboBox", this->speedComboBox);
        this->AddPos(L"positionTrackBar", this->positionTrackBar);
        this->AddPos(L"timeLabel", this->timeLabel);
    }

    void MainForm::RestoreOriginalControlPositions() {
        if (this->originalPositions == nullptr) return;
        this->SetPos(L"previousButton", this->previousButton);
        this->SetPos(L"skipBackwardButton", this->skipBackwardButton);
        if (this->playButton != nullptr) this->SetPos(L"playButton", this->playButton);
        if (this->pauseButton != nullptr) this->SetPos(L"pauseButton", this->pauseButton);
        this->SetPos(L"skipForwardButton", this->skipForwardButton);
        this->SetPos(L"nextButton", this->nextButton);
        this->SetPos(L"resetButton", this->resetButton);
        this->SetPos(L"fullScreenButton", this->fullScreenButton);
        this->SetPos(L"shuffleButton", this->shuffleButton);
        this->SetPos(L"loopButton", this->loopButton);
        this->SetPos(L"volumeLabel", this->volumeLabel);
        this->SetPos(L"volumeTrackBar", this->volumeTrackBar);
        this->SetPos(L"speedLabel", this->speedLabel);
        this->SetPos(L"speedComboBox", this->speedComboBox);
        this->SetPos(L"positionTrackBar", this->positionTrackBar);
        this->SetPos(L"timeLabel", this->timeLabel);
    }

    void MainForm::AddPos(System::String^ key, System::Windows::Forms::Control^ c) {
        if (c != nullptr && this->originalPositions != nullptr && !this->originalPositions->ContainsKey(key)) {
            this->originalPositions->Add(key, c->Location);
        }
    }

    void MainForm::SetPos(System::String^ key, System::Windows::Forms::Control^ c) {
        if (c != nullptr && this->originalPositions != nullptr && this->originalPositions->ContainsKey(key)) {
            c->Location = this->originalPositions[key];
        }
    }
}