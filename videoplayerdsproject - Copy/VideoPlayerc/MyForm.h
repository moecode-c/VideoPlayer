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
        System::Windows::Forms::Button^ btnMoreOptions;  // NEW - from second code
        System::Windows::Forms::Button^ btnBackToMenu;
        System::Windows::Forms::Panel^ videoPanel;
        System::Windows::Forms::ListBox^ listBox2;
        System::Windows::Forms::PictureBox^ pictureBoxBackground;
        System::Windows::Forms::ContextMenuStrip^ moreOptionsMenu;  // NEW - from second code
        AxWMPLib::AxWindowsMediaPlayer^ mediaPlayer;
        VideoList^ videoList;
        bool isPlaying;
        bool isDraggingSlider;

        // Extra controls (added features from first code)
        System::Windows::Forms::Panel^ controlPanel;
        System::Windows::Forms::Button^ playButton;
        System::Windows::Forms::Button^ pauseButton;
        System::Windows::Forms::Button^ nextButton;
        System::Windows::Forms::Button^ previousButton;
        System::Windows::Forms::Button^ shuffleButton;
        System::Windows::Forms::Button^ fullScreenButton;
        System::Windows::Forms::Button^ loopButton;
        System::Windows::Forms::Button^ skipForwardButton;
        System::Windows::Forms::Button^ skipBackwardButton;
        System::Windows::Forms::Button^ resetButton;
        System::Windows::Forms::TrackBar^ positionTrackBar;
        System::Windows::Forms::TrackBar^ volumeTrackBar;
        System::Windows::Forms::Label^ timeLabel;
        System::Windows::Forms::ComboBox^ speedComboBox;
        System::Windows::Forms::Label^ playlistInfoLabel;
        System::Windows::Forms::PictureBox^ volumeIcon;
        System::Windows::Forms::PictureBox^ speedIcon;
        System::Windows::Forms::Timer^ timer;
        bool isFullscreen;
        enum class LoopMode { Off, One, All };
        LoopMode loopMode;
        System::Collections::Generic::Dictionary<String^, System::Drawing::Point>^ originalPositions;

    public:
        MainForm(void)
        {
            InitializeComponent();
            // Ensure ActiveX control is fully created to avoid InvalidActiveXStateException
            this->mediaPlayer->CreateControl();
            this->mediaPlayer->Dock = System::Windows::Forms::DockStyle::Fill;
            this->mediaPlayer->stretchToFit = true;
            this->mediaPlayer->uiMode = "none";
            videoList = gcnew VideoList();
            isPlaying = false;
            isDraggingSlider = false;
            LoadBackgroundImage();
            LoadButtonImages();
            CreateMoreOptionsMenu();  // NEW - from second code
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


        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
            this->btnDelete = (gcnew System::Windows::Forms::Button());
            this->btnUpload = (gcnew System::Windows::Forms::Button());
            this->btnPlay = (gcnew System::Windows::Forms::Button());
            this->btnMoreOptions = (gcnew System::Windows::Forms::Button());
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
            this->volumeIcon = (gcnew System::Windows::Forms::PictureBox());
            this->volumeTrackBar = (gcnew System::Windows::Forms::TrackBar());
            this->speedIcon = (gcnew System::Windows::Forms::PictureBox());
            this->speedComboBox = (gcnew System::Windows::Forms::ComboBox());
            this->playlistInfoLabel = (gcnew System::Windows::Forms::Label());
            this->timer = (gcnew System::Windows::Forms::Timer(this->components));
            this->listBox2 = (gcnew System::Windows::Forms::ListBox());
            this->pictureBoxBackground = (gcnew System::Windows::Forms::PictureBox());
            this->moreOptionsMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->videoPanel->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mediaPlayer))->BeginInit();
            this->controlPanel->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->positionTrackBar))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeIcon))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeTrackBar))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->speedIcon))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->BeginInit();
            this->SuspendLayout();
            // 
            // btnDelete
            // 
            this->btnDelete->Location = System::Drawing::Point(279, 342);
            this->btnDelete->Name = L"btnDelete";
            this->btnDelete->Size = System::Drawing::Size(68, 54);
            this->btnDelete->TabIndex = 1;
            this->btnDelete->Text = L"Delete";
            this->btnDelete->UseVisualStyleBackColor = true;
            this->btnDelete->Click += gcnew System::EventHandler(this, &MainForm::btnDelete_Click);
            // 
            // btnUpload
            // 
            this->btnUpload->Location = System::Drawing::Point(386, 342);
            this->btnUpload->Name = L"btnUpload";
            this->btnUpload->Size = System::Drawing::Size(70, 63);
            this->btnUpload->TabIndex = 2;
            this->btnUpload->Text = L"Upload";
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
            // btnMoreOptions
            // 
            this->btnMoreOptions->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
            this->btnMoreOptions->Location = System::Drawing::Point(500, 112);
            this->btnMoreOptions->Name = L"btnMoreOptions";
            this->btnMoreOptions->Size = System::Drawing::Size(100, 40);
            this->btnMoreOptions->TabIndex = 5;
            this->btnMoreOptions->Text = L"⋮";
            this->btnMoreOptions->UseVisualStyleBackColor = true;
            this->btnMoreOptions->Click += gcnew System::EventHandler(this, &MainForm::btnMoreOptions_Click);
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
            this->videoPanel->Controls->Add(this->btnBackToMenu);
            this->videoPanel->Location = System::Drawing::Point(50, 12);
            this->videoPanel->Name = L"videoPanel";
            this->videoPanel->Size = System::Drawing::Size(640, 384);
            this->videoPanel->TabIndex = 9;
            this->videoPanel->Visible = false;
            // 
            // mediaPlayer
            // 
            this->mediaPlayer->Enabled = true;
            this->mediaPlayer->Location = System::Drawing::Point(0, 0);
            this->mediaPlayer->Name = L"mediaPlayer";
            this->mediaPlayer->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"mediaPlayer.OcxState")));
            // Make video fill the video panel
            this->mediaPlayer->Dock = System::Windows::Forms::DockStyle::Fill;
            this->mediaPlayer->TabIndex = 0;
            this->mediaPlayer->PlayStateChange += gcnew AxWMPLib::_WMPOCXEvents_PlayStateChangeEventHandler(this, &MainForm::mediaPlayer_PlayStateChange);
            // 
            // controlPanel
            // 
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
            this->controlPanel->Controls->Add(this->volumeIcon);
            this->controlPanel->Controls->Add(this->volumeTrackBar);
            this->controlPanel->Controls->Add(this->speedIcon);
            this->controlPanel->Controls->Add(this->speedComboBox);
            this->controlPanel->Cursor = System::Windows::Forms::Cursors::Hand;
            this->controlPanel->Location = System::Drawing::Point(50, 411);
            this->controlPanel->Name = L"controlPanel";
            this->controlPanel->Size = System::Drawing::Size(637, 227);
            this->controlPanel->TabIndex = 10;
            this->controlPanel->Visible = false;
            this->controlPanel->Resize += gcnew System::EventHandler(this, &MainForm::ControlPanel_Resize);
            // 
            // positionTrackBar
            // 
            this->positionTrackBar->AutoSize = false;
            this->positionTrackBar->LargeChange = 4;
            this->positionTrackBar->Location = System::Drawing::Point(11, 7);
            this->positionTrackBar->Margin = System::Windows::Forms::Padding(2);
            this->positionTrackBar->Maximum = 100;
            this->positionTrackBar->Name = L"positionTrackBar";
            this->positionTrackBar->Size = System::Drawing::Size(621, 27);
            this->positionTrackBar->TabIndex = 0;
            this->positionTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
            this->positionTrackBar->Scroll += gcnew System::EventHandler(this, &MainForm::PositionTrackBar_Scroll);
            this->positionTrackBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::PositionTrackBar_MouseDown);
            this->positionTrackBar->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::PositionTrackBar_MouseUp);
            // 
            // timeLabel
            // 
            this->timeLabel->ForeColor = System::Drawing::Color::White;
            this->timeLabel->Location = System::Drawing::Point(179, 27);
            this->timeLabel->Name = L"timeLabel";
            this->timeLabel->Size = System::Drawing::Size(293, 28);
            this->timeLabel->TabIndex = 1;
            this->timeLabel->Text = L"00:00 / 00:00";
            this->timeLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            // 
            // playButton
            // 
            this->playButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->playButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(46)), static_cast<System::Int32>(static_cast<System::Byte>(204)),
                static_cast<System::Int32>(static_cast<System::Byte>(113)));
            this->playButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"playButton.BackgroundImage")));
            this->playButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->playButton->Cursor = System::Windows::Forms::Cursors::Hand;
            this->playButton->FlatAppearance->BorderSize = 0;
            this->playButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->playButton->ForeColor = System::Drawing::Color::White;
            this->playButton->Location = System::Drawing::Point(288, 65);
            this->playButton->Name = L"playButton";
            this->playButton->Size = System::Drawing::Size(98, 93);
            this->playButton->TabIndex = 2;
            this->playButton->UseVisualStyleBackColor = false;
            this->playButton->Click += gcnew System::EventHandler(this, &MainForm::PlayButton_Click);
            // 
            // pauseButton
            // 
            this->pauseButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->pauseButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(231)), static_cast<System::Int32>(static_cast<System::Byte>(76)),
                static_cast<System::Int32>(static_cast<System::Byte>(60)));
            this->pauseButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pauseButton.BackgroundImage")));
            this->pauseButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->pauseButton->FlatAppearance->BorderSize = 0;
            this->pauseButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->pauseButton->ForeColor = System::Drawing::Color::White;
            this->pauseButton->Location = System::Drawing::Point(289, 57);
            this->pauseButton->Name = L"pauseButton";
            this->pauseButton->Size = System::Drawing::Size(97, 100);
            this->pauseButton->TabIndex = 3;
            this->pauseButton->UseVisualStyleBackColor = false;
            this->pauseButton->Visible = false;
            this->pauseButton->Click += gcnew System::EventHandler(this, &MainForm::PauseButton_Click);
            // 
            // previousButton
            // 
            this->previousButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->previousButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(152)),
                static_cast<System::Int32>(static_cast<System::Byte>(219)));
            this->previousButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"previousButton.BackgroundImage")));
            this->previousButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->previousButton->FlatAppearance->BorderSize = 0;
            this->previousButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->previousButton->ForeColor = System::Drawing::Color::White;
            this->previousButton->Location = System::Drawing::Point(156, 60);
            this->previousButton->Name = L"previousButton";
            this->previousButton->Size = System::Drawing::Size(74, 95);
            this->previousButton->TabIndex = 4;
            this->previousButton->UseVisualStyleBackColor = false;
            this->previousButton->Click += gcnew System::EventHandler(this, &MainForm::PreviousButton_Click);
            // 
            // nextButton
            // 
            this->nextButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->nextButton->BackColor = System::Drawing::Color::White;
            this->nextButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"nextButton.BackgroundImage")));
            this->nextButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->nextButton->FlatAppearance->BorderSize = 0;
            this->nextButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->nextButton->ForeColor = System::Drawing::Color::White;
            this->nextButton->Location = System::Drawing::Point(448, 52);
            this->nextButton->Name = L"nextButton";
            this->nextButton->Size = System::Drawing::Size(66, 107);
            this->nextButton->TabIndex = 5;
            this->nextButton->UseVisualStyleBackColor = false;
            this->nextButton->Click += gcnew System::EventHandler(this, &MainForm::NextButton_Click);
            // 
            // resetButton
            // 
            this->resetButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->resetButton->BackColor = System::Drawing::Color::Transparent;
            this->resetButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"resetButton.BackgroundImage")));
            this->resetButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->resetButton->FlatAppearance->BorderSize = 0;
            this->resetButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->resetButton->ForeColor = System::Drawing::Color::White;
            this->resetButton->Location = System::Drawing::Point(517, 84);
            this->resetButton->Name = L"resetButton";
            this->resetButton->Size = System::Drawing::Size(46, 51);
            this->resetButton->TabIndex = 6;
            this->resetButton->UseVisualStyleBackColor = false;
            this->resetButton->Click += gcnew System::EventHandler(this, &MainForm::ResetButton_Click);
            // 
            // shuffleButton
            // 
            this->shuffleButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->shuffleButton->AutoSize = true;
            this->shuffleButton->BackColor = System::Drawing::Color::Transparent;
            this->shuffleButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"shuffleButton.BackgroundImage")));
            this->shuffleButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->shuffleButton->FlatAppearance->BorderSize = 0;
            this->shuffleButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->shuffleButton->ForeColor = System::Drawing::Color::Transparent;
            this->shuffleButton->Location = System::Drawing::Point(79, 84);
            this->shuffleButton->Name = L"shuffleButton";
            this->shuffleButton->Size = System::Drawing::Size(74, 46);
            this->shuffleButton->TabIndex = 7;
            this->shuffleButton->UseVisualStyleBackColor = false;
            this->shuffleButton->Click += gcnew System::EventHandler(this, &MainForm::ShuffleButton_Click);
            // 
            // fullScreenButton
            // 
            this->fullScreenButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->fullScreenButton->BackColor = System::Drawing::Color::Transparent;
            this->fullScreenButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"fullScreenButton.BackgroundImage")));
            this->fullScreenButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->fullScreenButton->FlatAppearance->BorderSize = 0;
            this->fullScreenButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->fullScreenButton->ForeColor = System::Drawing::Color::White;
            this->fullScreenButton->Location = System::Drawing::Point(579, 158);
            this->fullScreenButton->Name = L"fullScreenButton";
            this->fullScreenButton->Size = System::Drawing::Size(50, 48);
            this->fullScreenButton->TabIndex = 13;
            this->fullScreenButton->UseVisualStyleBackColor = false;
            this->fullScreenButton->Click += gcnew System::EventHandler(this, &MainForm::FullScreenButton_Click);
            // 
            // loopButton
            // 
            this->loopButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->loopButton->BackColor = System::Drawing::Color::Transparent;
            this->loopButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"loopButton.BackgroundImage")));
            this->loopButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->loopButton->FlatAppearance->BorderSize = 0;
            this->loopButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->loopButton->ForeColor = System::Drawing::Color::White;
            this->loopButton->Location = System::Drawing::Point(154, 163);
            this->loopButton->Name = L"loopButton";
            this->loopButton->Size = System::Drawing::Size(67, 39);
            this->loopButton->TabIndex = 14;
            this->loopButton->UseVisualStyleBackColor = false;
            this->loopButton->Click += gcnew System::EventHandler(this, &MainForm::LoopButton_Click);
            // 
            // skipForwardButton
            // 
            this->skipForwardButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->skipForwardButton->BackColor = System::Drawing::Color::DarkSlateGray;
            this->skipForwardButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"skipForwardButton.BackgroundImage")));
            this->skipForwardButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->skipForwardButton->FlatAppearance->BorderSize = 0;
            this->skipForwardButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->skipForwardButton->ForeColor = System::Drawing::Color::White;
            this->skipForwardButton->Location = System::Drawing::Point(373, 51);
            this->skipForwardButton->Name = L"skipForwardButton";
            this->skipForwardButton->Size = System::Drawing::Size(80, 111);
            this->skipForwardButton->TabIndex = 15;
            this->skipForwardButton->UseVisualStyleBackColor = false;
            this->skipForwardButton->Click += gcnew System::EventHandler(this, &MainForm::SkipForwardButton_Click);
            // 
            // skipBackwardButton
            // 
            this->skipBackwardButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->skipBackwardButton->BackColor = System::Drawing::Color::DarkSlateGray;
            this->skipBackwardButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"skipBackwardButton.BackgroundImage")));
            this->skipBackwardButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->skipBackwardButton->FlatAppearance->BorderSize = 0;
            this->skipBackwardButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->skipBackwardButton->ForeColor = System::Drawing::Color::White;
            this->skipBackwardButton->Location = System::Drawing::Point(227, 43);
            this->skipBackwardButton->Name = L"skipBackwardButton";
            this->skipBackwardButton->Size = System::Drawing::Size(80, 127);
            this->skipBackwardButton->TabIndex = 16;
            this->skipBackwardButton->UseVisualStyleBackColor = false;
            this->skipBackwardButton->Click += gcnew System::EventHandler(this, &MainForm::SkipBackwardButton_Click);
            // 
            // volumeIcon
            // 
            this->volumeIcon->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"volumeIcon.BackgroundImage")));
            this->volumeIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->volumeIcon->Location = System::Drawing::Point(290, 164);
            this->volumeIcon->Name = L"volumeIcon";
            this->volumeIcon->Size = System::Drawing::Size(48, 49);
            this->volumeIcon->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->volumeIcon->TabIndex = 17;
            this->volumeIcon->TabStop = false;
            // 
            // volumeTrackBar
            // 
            this->volumeTrackBar->AutoSize = false;
            this->volumeTrackBar->BackColor = System::Drawing::Color::Snow;
            this->volumeTrackBar->Location = System::Drawing::Point(336, 174);
            this->volumeTrackBar->Maximum = 100;
            this->volumeTrackBar->Name = L"volumeTrackBar";
            this->volumeTrackBar->Size = System::Drawing::Size(101, 32);
            this->volumeTrackBar->TabIndex = 9;
            this->volumeTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
            this->volumeTrackBar->Value = 50;
            this->volumeTrackBar->Scroll += gcnew System::EventHandler(this, &MainForm::VolumeTrackBar_Scroll);
            // 
            // speedIcon
            // 
            this->speedIcon->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"speedIcon.BackgroundImage")));
            this->speedIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->speedIcon->Location = System::Drawing::Point(18, 121);
            this->speedIcon->Name = L"speedIcon";
            this->speedIcon->Size = System::Drawing::Size(61, 41);
            this->speedIcon->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->speedIcon->TabIndex = 19;
            this->speedIcon->TabStop = false;
            this->speedIcon->Click += gcnew System::EventHandler(this, &MainForm::SpeedIcon_Click);
            // 
            // speedComboBox
            // 
            this->speedComboBox->AllowDrop = true;
            this->speedComboBox->BackColor = System::Drawing::Color::LightCyan;
            this->speedComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->speedComboBox->FormattingEnabled = true;
            this->speedComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(5) { L"0.5x", L"1.0x", L"1.25x", L"1.5x", L"2.0x" });
            this->speedComboBox->Location = System::Drawing::Point(0, 166);
            this->speedComboBox->Name = L"speedComboBox";
            this->speedComboBox->Size = System::Drawing::Size(97, 24);
            this->speedComboBox->TabIndex = 12;
            this->speedComboBox->Visible = false;
            this->speedComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::SpeedComboBox_SelectedIndexChanged);
            this->speedComboBox->DropDownClosed += gcnew System::EventHandler(this, &MainForm::SpeedComboBox_DropDownClosed);
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
            this->listBox2->Size = System::Drawing::Size(177, 196);
            this->listBox2->TabIndex = 3;
            this->listBox2->DoubleClick += gcnew System::EventHandler(this, &MainForm::listBox2_DoubleClick);
            // 
            // pictureBoxBackground
            // 
            this->pictureBoxBackground->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxBackground->Location = System::Drawing::Point(0, 0);
            this->pictureBoxBackground->Name = L"pictureBoxBackground";
            this->pictureBoxBackground->Size = System::Drawing::Size(742, 645);
            this->pictureBoxBackground->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->pictureBoxBackground->TabIndex = 0;
            this->pictureBoxBackground->TabStop = false;
            // 
            // moreOptionsMenu
            // 
            this->moreOptionsMenu->ImageScalingSize = System::Drawing::Size(20, 20);
            this->moreOptionsMenu->Name = L"moreOptionsMenu";
            this->moreOptionsMenu->Size = System::Drawing::Size(61, 4);
            // 
            // MainForm
            // 
            this->ClientSize = System::Drawing::Size(742, 645);
            this->Controls->Add(this->videoPanel);
            this->Controls->Add(this->controlPanel);
            this->Controls->Add(this->btnMoreOptions);
            this->Controls->Add(this->btnPlay);
            this->Controls->Add(this->listBox2);
            this->Controls->Add(this->btnUpload);
            this->Controls->Add(this->btnDelete);
            this->Controls->Add(this->pictureBoxBackground);
            this->Name = L"MainForm";
            this->Text = L"Video Player";
            this->videoPanel->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mediaPlayer))->EndInit();
            this->controlPanel->ResumeLayout(false);
            this->controlPanel->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->positionTrackBar))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeIcon))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeTrackBar))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->speedIcon))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->EndInit();
            this->ResumeLayout(false);

        }

        // ========== EXACT FUNCTIONS FROM SECOND CODE ==========
        void CreateMoreOptionsMenu()
        {
            // Create menu items
            ToolStripMenuItem^ sortAlphaItem = gcnew ToolStripMenuItem("Sort A-Z");
            sortAlphaItem->Click += gcnew System::EventHandler(this, &MainForm::sortAlpha_Click);

            ToolStripMenuItem^ sortTimeItem = gcnew ToolStripMenuItem("Sort by Time Added");
            sortTimeItem->Click += gcnew System::EventHandler(this, &MainForm::sortTime_Click);

            ToolStripSeparator^ separator = gcnew ToolStripSeparator();

            ToolStripMenuItem^ clearAllItem = gcnew ToolStripMenuItem("Clear All");
            clearAllItem->Click += gcnew System::EventHandler(this, &MainForm::clearAll_Click);

            // Add items to menu
            moreOptionsMenu->Items->Add(sortAlphaItem);
            moreOptionsMenu->Items->Add(sortTimeItem);
            moreOptionsMenu->Items->Add(separator);
            moreOptionsMenu->Items->Add(clearAllItem);
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

            // Position the More Options button (3 dots)
            double newButtonWidthPercent = 13.0;
            double newButtonHeightPercent = 9.0;
            int newButtonWidth = (int)(this->ClientSize.Width * newButtonWidthPercent / 100.0);
            int newButtonHeight = (int)(this->ClientSize.Height * newButtonHeightPercent / 100.0);

            double newButtonLeftPercent = 67.0;
            int newButtonLeft = (int)(this->ClientSize.Width * newButtonLeftPercent / 100.0);

            double newButtonTopPercent = 24.5;
            int newButtonTop = (int)(this->ClientSize.Height * newButtonTopPercent / 100.0);

            btnMoreOptions->Location = System::Drawing::Point(newButtonLeft, newButtonTop);
            btnMoreOptions->Size = System::Drawing::Size(newButtonWidth, newButtonHeight);
        }

        void PositionMediaPlayer()
        {
            int panelWidth = (int)(this->ClientSize.Width * 0.85);
            int panelHeight = (int)(this->ClientSize.Height * 0.85);
            int panelLeft = (this->ClientSize.Width - panelWidth) / 2;
            int panelTop = (this->ClientSize.Height - panelHeight) / 2;

            videoPanel->Location = System::Drawing::Point(panelLeft, panelTop);
            videoPanel->Size = System::Drawing::Size(panelWidth, panelHeight);

            btnBackToMenu->Location = System::Drawing::Point(10, 10);
            // MediaPlayer is docked to Fill; no manual location/size needed
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

                // Load More Options button image
                String^ filterImagePath = Path::Combine(basePath, "Images\\filterbtn.jpeg");
                if (File::Exists(filterImagePath))
                {
                    btnMoreOptions->BackgroundImage = Image::FromFile(filterImagePath);
                    btnMoreOptions->BackgroundImageLayout = ImageLayout::Stretch;
                    btnMoreOptions->Text = "";
                    btnMoreOptions->FlatStyle = FlatStyle::Flat;
                    btnMoreOptions->FlatAppearance->BorderSize = 0;
                    btnMoreOptions->BackColor = Color::Transparent;
                }
                else
                {
                    // Fallback styling if image not found
                    btnMoreOptions->FlatStyle = FlatStyle::Flat;
                    btnMoreOptions->FlatAppearance->BorderSize = 1;
                    btnMoreOptions->BackColor = Color::FromArgb(240, 240, 240);
                    btnMoreOptions->ForeColor = Color::Black;
                }

                // ========== ADDITIONAL IMAGES FROM FIRST CODE ==========
                String^ fullScreenImagePath = Path::Combine(basePath, "Images\\fullscreen.png");
                if (File::Exists(fullScreenImagePath))
                {
                    fullScreenButton->BackgroundImage = Image::FromFile(fullScreenImagePath);
                    fullScreenButton->BackgroundImageLayout = ImageLayout::Stretch;
                    fullScreenButton->Text = "";
                    fullScreenButton->FlatStyle = FlatStyle::Flat;
                    fullScreenButton->FlatAppearance->BorderSize = 0;
                    fullScreenButton->BackColor = Color::Transparent;
                    fullScreenButton->Size = System::Drawing::Size(60, 55);
                }

                String^ playBtnImg = Path::Combine(basePath, "Images\\playBtn.png");
                if (playButton != nullptr && File::Exists(playBtnImg)) {
                    playButton->BackgroundImage = Image::FromFile(playBtnImg);
                    playButton->BackgroundImageLayout = ImageLayout::Stretch;
                    playButton->Text = "";
                    playButton->FlatStyle = FlatStyle::Flat;
                    playButton->FlatAppearance->BorderSize = 0;
                    playButton->BackColor = Color::Transparent;
                }

                String^ pauseBtnImg = Path::Combine(basePath, "Images\\pausebtn.png");
                if (pauseButton != nullptr && File::Exists(pauseBtnImg)) {
                    pauseButton->BackgroundImage = Image::FromFile(pauseBtnImg);
                    pauseButton->BackgroundImageLayout = ImageLayout::Stretch;
                    pauseButton->Text = "";
                    pauseButton->FlatStyle = FlatStyle::Flat;
                    pauseButton->FlatAppearance->BorderSize = 0;
                    pauseButton->BackColor = Color::Transparent;
                }

                String^ prevImg = Path::Combine(basePath, "Images\\prevbtn.png");
                if (previousButton != nullptr && File::Exists(prevImg)) {
                    previousButton->BackgroundImage = Image::FromFile(prevImg);
                    previousButton->BackgroundImageLayout = ImageLayout::Stretch;
                    previousButton->Text = "";
                    previousButton->FlatStyle = FlatStyle::Flat;
                    previousButton->FlatAppearance->BorderSize = 0;
                    previousButton->BackColor = Color::Transparent;
                }

                String^ nextImg = Path::Combine(basePath, "Images\\nextBtn.png");
                if (nextButton != nullptr && File::Exists(nextImg)) {
                    nextButton->BackgroundImage = Image::FromFile(nextImg);
                    nextButton->BackgroundImageLayout = ImageLayout::Stretch;
                    nextButton->Text = "";
                    nextButton->FlatStyle = FlatStyle::Flat;
                    nextButton->FlatAppearance->BorderSize = 0;
                    nextButton->BackColor = Color::Transparent;
                }

                String^ resetImg = Path::Combine(basePath, "Images\\resetBtn.png");
                if (resetButton != nullptr && File::Exists(resetImg)) {
                    resetButton->BackgroundImage = Image::FromFile(resetImg);
                    resetButton->BackgroundImageLayout = ImageLayout::Stretch;
                    resetButton->Text = "";
                    resetButton->FlatStyle = FlatStyle::Flat;
                    resetButton->FlatAppearance->BorderSize = 0;
                    resetButton->BackColor = Color::Transparent;
                    resetButton->BringToFront();
                    resetButton->Size = System::Drawing::Size(46, 45);
                }

                String^ shuffleImg = Path::Combine(basePath, "Images\\shuffleBtn.png");
                if (shuffleButton != nullptr && File::Exists(shuffleImg)) {
                    shuffleButton->BackgroundImage = Image::FromFile(shuffleImg);
                    shuffleButton->BackgroundImageLayout = ImageLayout::Stretch;
                    shuffleButton->Text = "";
                    shuffleButton->FlatStyle = FlatStyle::Flat;
                    shuffleButton->FlatAppearance->BorderSize = 0;
                    shuffleButton->BackColor = Color::Transparent;
                    shuffleButton->Size = System::Drawing::Size(60, 49);
                    shuffleButton->AccessibilityObject->Name = "Shuffle Button";
                }

                String^ loopImg = Path::Combine(basePath, "Images\\loopBtn.png");
                if (loopButton != nullptr && File::Exists(loopImg)) {
                    loopButton->BackgroundImage = Image::FromFile(loopImg);
                    loopButton->BackgroundImageLayout = ImageLayout::Stretch;
                    loopButton->Text = "";
                    loopButton->FlatStyle = FlatStyle::Flat;
                    loopButton->FlatAppearance->BorderSize = 0;
                    loopButton->BackColor = Color::Transparent;
                    loopButton->BringToFront();
                    loopButton->Size = System::Drawing::Size(79, 50);
                }

                String^ forwardImg = Path::Combine(basePath, "Images\\skipforwardBtn.png");
                if (skipForwardButton != nullptr && File::Exists(forwardImg)) {
                    skipForwardButton->BackgroundImage = Image::FromFile(forwardImg);
                    skipForwardButton->BackgroundImageLayout = ImageLayout::Stretch;
                    skipForwardButton->Text = "";
                    skipForwardButton->FlatStyle = FlatStyle::Flat;
                    skipForwardButton->FlatAppearance->BorderSize = 0;
                    skipForwardButton->BackColor = Color::Transparent;
                }

                String^ backwardImg = Path::Combine(basePath, "Images\\skipbackwardBtn.png");
                if (skipBackwardButton != nullptr && File::Exists(backwardImg)) {
                    skipBackwardButton->BackgroundImage = Image::FromFile(backwardImg);
                    skipBackwardButton->BackgroundImageLayout = ImageLayout::Stretch;
                    skipBackwardButton->Text = "";
                    skipBackwardButton->FlatStyle = FlatStyle::Flat;
                    skipBackwardButton->FlatAppearance->BorderSize = 0;
                    skipBackwardButton->BackColor = Color::Transparent;
                }

                String^ volumeIconPath = Path::Combine(basePath, "Images\\volumeBtn.png");
                if (volumeIcon != nullptr && File::Exists(volumeIconPath)) {
                    volumeIcon->Image = Image::FromFile(volumeIconPath);
                    volumeIcon->SizeMode = PictureBoxSizeMode::StretchImage;
                    volumeIcon->Visible = true;
                }

                String^ speedIconPath = Path::Combine(basePath, "Images\\speedBtn.png");
                if (speedIcon != nullptr && File::Exists(speedIconPath)) {
                    speedIcon->Image = Image::FromFile(speedIconPath);
                    speedIcon->SizeMode = PictureBoxSizeMode::StretchImage;
                    speedIcon->Visible = true;
                }
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

        void ShowVideoPanel()
        {
            videoPanel->Visible = true;
            videoPanel->BringToFront();
            controlPanel->Visible = true;  // ADD THIS LINE
            controlPanel->BringToFront();  // ADD THIS LINE

            listBox2->Visible = false;
            btnDelete->Visible = false;
            btnUpload->Visible = false;
            btnPlay->Visible = false;
            btnMoreOptions->Visible = false;
        }

        void ShowMenu()
        {
            videoPanel->Visible = false;
            controlPanel->Visible = false;  // ADD THIS LINE

            listBox2->Visible = true;
            btnDelete->Visible = true;
            btnUpload->Visible = true;
            btnPlay->Visible = true;
            btnMoreOptions->Visible = true;
        }

        System::Void btnBackToMenu_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (isPlaying)
            {
                mediaPlayer->Ctlcontrols->stop();
                isPlaying = false;
            }

            ShowMenu();
        }

        System::Void btnMoreOptions_Click(System::Object^ sender, System::EventArgs^ e)
        {
            // Show the context menu below the button
            Point buttonLocation = btnMoreOptions->PointToScreen(Point(0, btnMoreOptions->Height));
            moreOptionsMenu->Show(buttonLocation);
        }

        System::Void sortAlpha_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->sortAlphabetically(listBox2);
        }

        System::Void sortTime_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->sortByTimeAdded(listBox2);
        }

        System::Void clearAll_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->clearAll(listBox2);
        }

        System::Void btnPlay_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex >= 0)
            {
                VideoList::Node^ selectedNode = videoList->getNodeAtDisplayIndex(listBox2->SelectedIndex, listBox2);

                if (selectedNode != nullptr)
                {
                    if (selectedNode->isPlaylistHeader)
                    {
                        videoList->togglePlaylistExpansion(listBox2->SelectedIndex, listBox2);
                        return;
                    }

                    videoList->setCurrentNode(selectedNode);
                }
            }

            String^ videoPath = videoList->getCurrentNodePath();

            if (videoPath != nullptr && File::Exists(videoPath))
            {
                mediaPlayer->URL = videoPath;
                ShowVideoPanel();
                mediaPlayer->Ctlcontrols->play();
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

        System::Void listBox2_DoubleClick(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex >= 0)
            {
                btnPlay_Click(sender, e);
            }
        }

        System::Void mediaPlayer_PlayStateChange(System::Object^ sender, AxWMPLib::_WMPOCXEvents_PlayStateChangeEvent^ e)
        {
            if (e->newState == 8) // MediaEnded
            {
                // ADD LOOP ONE LOGIC
                if (loopMode == LoopMode::One && mediaPlayer->currentMedia != nullptr)
                {
                    mediaPlayer->Ctlcontrols->currentPosition = 0;
                    mediaPlayer->Ctlcontrols->play();
                    ApplySelectedPlaybackRate();
                    isPlaying = true;
                    return;
                }

                String^ nextPath = videoList->nextVideo();
                if (nextPath != nullptr)
                {
                    mediaPlayer->URL = nextPath;
                    mediaPlayer->Ctlcontrols->play();
                    ApplySelectedPlaybackRate();  // ADD THIS
                    isPlaying = true;
                    controlPanel->Visible = true;  // ADD THIS
                }
                else
                {
                    // ADD LOOP ALL LOGIC
                    if (loopMode == LoopMode::All && listBox2->Items->Count > 0)
                    {
                        listBox2->SelectedIndex = 0;
                        videoList->setCurrentNode(0);
                        String^ path = videoList->getCurrentNodePath();
                        if (path != nullptr)
                        {
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
            else if (e->newState == 1) // Stopped
            {
                isPlaying = false;
                timer->Enabled = false;  // ADD THIS
            }
            else if (e->newState == 3) // Playing
            {
                isPlaying = true;
                controlPanel->Visible = true;  // ADD THIS
                timer->Interval = 200;  // ADD THIS
                timer->Enabled = true;  // ADD THIS
            }
        }

        System::Void btnUpload_Click(System::Object^ sender, System::EventArgs^ e)
        {
            // Create custom dialog
            Form^ uploadDialog = gcnew Form();
            uploadDialog->Text = "Upload Content";
            uploadDialog->Width = 400;
            uploadDialog->Height = 200;
            uploadDialog->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            uploadDialog->StartPosition = FormStartPosition::CenterParent;
            uploadDialog->MaximizeBox = false;
            uploadDialog->MinimizeBox = false;

            Label^ label = gcnew Label();
            label->Text = "What would you like to upload?";
            label->Location = System::Drawing::Point(20, 20);
            label->AutoSize = true;
            label->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11));

            Button^ btnVideo = gcnew Button();
            btnVideo->Text = "📹 Single Videos";
            btnVideo->Location = System::Drawing::Point(50, 70);
            btnVideo->Size = System::Drawing::Size(140, 50);
            btnVideo->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            btnVideo->DialogResult = System::Windows::Forms::DialogResult::Yes;

            Button^ btnPlaylist = gcnew Button();
            btnPlaylist->Text = "📂 Playlist";
            btnPlaylist->Location = System::Drawing::Point(210, 70);
            btnPlaylist->Size = System::Drawing::Size(140, 50);
            btnPlaylist->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            btnPlaylist->DialogResult = System::Windows::Forms::DialogResult::No;

            uploadDialog->Controls->Add(label);
            uploadDialog->Controls->Add(btnVideo);
            uploadDialog->Controls->Add(btnPlaylist);
            uploadDialog->AcceptButton = btnVideo;
            uploadDialog->CancelButton = btnPlaylist;

            System::Windows::Forms::DialogResult result = uploadDialog->ShowDialog();

            if (result == System::Windows::Forms::DialogResult::Yes)
            {
                // Upload individual videos
                OpenFileDialog^ ofd = gcnew OpenFileDialog();
                ofd->Filter = "Video Files|*.mp4;*.avi;*.mkv;*.mov;*.wmv;*.flv";
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



            else if (result == System::Windows::Forms::DialogResult::No)
            {
                // Upload playlist
                String^ playlistName = nullptr;

                Form^ inputForm = gcnew Form();
                inputForm->Text = "New Playlist";
                inputForm->Width = 350;
                inputForm->Height = 150;
                inputForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
                inputForm->StartPosition = FormStartPosition::CenterParent;
                inputForm->MaximizeBox = false;
                inputForm->MinimizeBox = false;

                Label^ label2 = gcnew Label();
                label2->Text = "Enter playlist name:";
                label2->Location = System::Drawing::Point(10, 20);
                label2->AutoSize = true;

                TextBox^ textBox = gcnew TextBox();
                textBox->Location = System::Drawing::Point(10, 45);
                textBox->Width = 310;

                Button^ btnOK = gcnew Button();
                btnOK->Text = "OK";
                btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
                btnOK->Location = System::Drawing::Point(150, 75);

                Button^ btnCancel = gcnew Button();
                btnCancel->Text = "Cancel";
                btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
                btnCancel->Location = System::Drawing::Point(235, 75);

                inputForm->Controls->Add(label2);
                inputForm->Controls->Add(textBox);
                inputForm->Controls->Add(btnOK);
                inputForm->Controls->Add(btnCancel);
                inputForm->AcceptButton = btnOK;
                inputForm->CancelButton = btnCancel;

                if (inputForm->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                {
                    playlistName = textBox->Text->Trim();

                    if (String::IsNullOrWhiteSpace(playlistName))
                    {
                        MessageBox::Show("Playlist name cannot be empty!", "Error",
                            MessageBoxButtons::OK, MessageBoxIcon::Error);
                        return;
                    }

                    OpenFileDialog^ ofd = gcnew OpenFileDialog();
                    ofd->Filter = "Video Files|*.mp4;*.avi;*.mkv;*.mov;*.wmv;*.flv";
                    ofd->Multiselect = true;
                    ofd->Title = "Select videos for playlist: " + playlistName;

                    if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                    {
                        if (ofd->FileNames->Length > 0)
                        {
                            videoList->addPlaylist(ofd->FileNames, playlistName, listBox2);
                        }
                    }
                }
            }
        }

        System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex == -1)
            {
                MessageBox::Show("Please select a video to delete.", "Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            videoList->removeItem(listBox2->SelectedIndex, listBox2);
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

        // ========== FUNCTIONS FROM FIRST CODE THAT ARE NOT IN SECOND CODE ==========
        void ArrangeControlsCentered()
        {
            if (controlPanel == nullptr) return;
            if (!isFullscreen) return; // Only center in fullscreen to preserve original layout in normal mode

            if (positionTrackBar != nullptr)
            {
                positionTrackBar->Width = controlPanel->ClientSize.Width - 20;
                positionTrackBar->Location = System::Drawing::Point(10, positionTrackBar->Location.Y);
            }
            if (timeLabel != nullptr)
            {
                timeLabel->Width = controlPanel->ClientSize.Width - 200;
                timeLabel->Location = System::Drawing::Point((controlPanel->ClientSize.Width - timeLabel->Width) / 2, timeLabel->Location.Y);
            }

            if (volumeIcon != nullptr && volumeTrackBar != nullptr)
            {
                volumeIcon->Location = System::Drawing::Point(10, volumeIcon->Location.Y);
                volumeTrackBar->Location = System::Drawing::Point(volumeIcon->Right + 12, volumeTrackBar->Location.Y);
            }
            if (loopButton != nullptr)
            {
                loopButton->Location = System::Drawing::Point(10, loopButton->Location.Y);
            }
            if (speedComboBox != nullptr)
            {
                int rightPadding = 10;
                speedComboBox->Location = System::Drawing::Point(controlPanel->ClientSize.Width - speedComboBox->Width - rightPadding, speedComboBox->Location.Y);
                if (speedIcon != nullptr)
                {
                    speedIcon->Location = System::Drawing::Point(speedComboBox->Left - speedIcon->Width - 8, speedIcon->Location.Y);
                }
            }
        }

        System::Void PlayButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (mediaPlayer->URL != nullptr && mediaPlayer->URL != "") {
                mediaPlayer->Ctlcontrols->play();
                ApplySelectedPlaybackRate();
                playButton->Visible = false;
                pauseButton->Visible = true;
                isPlaying = true;
                if (isFullscreen) ArrangeControlsCentered();
            }
        }

        System::Void PauseButton_Click(System::Object^ sender, System::EventArgs^ e) {
            mediaPlayer->Ctlcontrols->pause();
            pauseButton->Visible = false;
            playButton->Visible = true;
            isPlaying = false;
            if (isFullscreen) ArrangeControlsCentered();
        }

        System::Void NextButton_Click(System::Object^ sender, System::EventArgs^ e) {
            String^ nextPath = videoList->nextVideo();
            if (nextPath != nullptr) {
                mediaPlayer->URL = nextPath;
                mediaPlayer->Ctlcontrols->play();
                isPlaying = true;
            }
        }

        System::Void SpeedComboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            ApplySelectedPlaybackRate();
            if (speedComboBox != nullptr) speedComboBox->Visible = false;
        }

        System::Void SpeedComboBox_DropDownClosed(System::Object^ sender, System::EventArgs^ e) {
            if (speedComboBox != nullptr) speedComboBox->Visible = false;
        }

        System::Void SpeedIcon_Click(System::Object^ sender, System::EventArgs^ e) {
            if (speedComboBox == nullptr) return;
            speedComboBox->Visible = true;
            speedComboBox->Focus();
            speedComboBox->DroppedDown = true;
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

        System::Void PreviousButton_Click(System::Object^ sender, System::EventArgs^ e) {
            int idx = listBox2->SelectedIndex;
            if (idx <= 0 && listBox2->Items->Count > 0) {
                idx = 0;
            }
            else if (idx > 0) {
                idx = idx - 1;
            }
            else {
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

        System::Void ResetButton_Click(System::Object^ sender, System::EventArgs^ e) {
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

        System::Void PositionTrackBar_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            isDraggingSlider = true;
        }

        System::Void PositionTrackBar_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            isDraggingSlider = false;
            if (mediaPlayer->currentMedia != nullptr) {
                double duration = mediaPlayer->currentMedia->duration;
                double newPosition = (positionTrackBar->Value / 100.0) * duration;
                mediaPlayer->Ctlcontrols->currentPosition = newPosition;
            }
        }

        System::Void PositionTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e) {
            if (!isDraggingSlider && mediaPlayer->currentMedia != nullptr) {
                double duration = mediaPlayer->currentMedia->duration;
                double newPosition = (positionTrackBar->Value / 100.0) * duration;
                mediaPlayer->Ctlcontrols->currentPosition = newPosition;
            }
        }

        System::Void VolumeTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e) {
            mediaPlayer->settings->volume = volumeTrackBar->Value;
        }

        System::Void Timer_Tick(System::Object^ sender, System::EventArgs^ e) {
            if (mediaPlayer->currentMedia != nullptr && !isDraggingSlider) {
                double duration = mediaPlayer->currentMedia->duration;
                double position = mediaPlayer->Ctlcontrols->currentPosition;

                if (duration > 0) {
                    int percentage = (int)((position / duration) * 100);
                    positionTrackBar->Value = Math::Min(percentage, 100);
                }

                timeLabel->Text = FormatTime(position) + " / " + FormatTime(duration);
            }
        }

        System::Void SkipForwardButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (mediaPlayer->currentMedia == nullptr) return;
            double duration = mediaPlayer->currentMedia->duration;
            double pos = mediaPlayer->Ctlcontrols->currentPosition;
            double newPos = Math::Min(duration, pos + 5.0);
            mediaPlayer->Ctlcontrols->currentPosition = newPos;
        }

        System::Void SkipBackwardButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (mediaPlayer->currentMedia == nullptr) return;
            double pos = mediaPlayer->Ctlcontrols->currentPosition;
            double newPos = Math::Max(0.0, pos - 5.0);
            mediaPlayer->Ctlcontrols->currentPosition = newPos;
        }

        System::Void LoopButton_Click(System::Object^ sender, System::EventArgs^ e) {
            // Cycle through Off -> One -> All -> Off
            if (loopMode == LoopMode::Off) loopMode = LoopMode::One;
            else if (loopMode == LoopMode::One) loopMode = LoopMode::All;
            else loopMode = LoopMode::Off;

            if (loopMode == LoopMode::Off) {
                loopButton->BackColor = System::Drawing::Color::Gray;
            }
            else if (loopMode == LoopMode::One) {
                loopButton->BackColor = System::Drawing::Color::FromArgb(76, 175, 80);
            }
            else {
                loopButton->BackColor = System::Drawing::Color::FromArgb(52, 152, 219);
            }
        }

        System::Void FullScreenButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (!isFullscreen) {
                this->WindowState = FormWindowState::Maximized;
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
                videoPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                // Ensure media player fills the panel and uses WMP scaling
                mediaPlayer->Dock = System::Windows::Forms::DockStyle::Fill;
                mediaPlayer->stretchToFit = true;
                mediaPlayer->Location = System::Drawing::Point(0, 0);
                mediaPlayer->Size = videoPanel->ClientSize;

                // Dock control panel to bottom of the form and ensure it's on top of the video
                controlPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
                controlPanel->Visible = true;
                videoPanel->BringToFront();
                controlPanel->BringToFront();
                videoPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
                this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
                isFullscreen = true;
                ArrangeControlsCentered();
            }
            else {
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
                this->WindowState = FormWindowState::Normal;
                controlPanel->Dock = System::Windows::Forms::DockStyle::None;
                videoPanel->Dock = System::Windows::Forms::DockStyle::None;
                // Restore media player to fill panel (panel resized by PositionMediaPlayer)
                mediaPlayer->Dock = System::Windows::Forms::DockStyle::Fill;
                mediaPlayer->stretchToFit = true;
                videoPanel->MouseMove -= gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
                this->MouseMove -= gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
                PositionMediaPlayer();
                isFullscreen = false;
                controlPanel->Visible = true;
                controlPanel->BringToFront();
                RestoreOriginalControlPositions();
                ArrangeControlsCentered();
            }
        }

        System::Void VideoPanel_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            if (!isFullscreen) return;
            int threshold = 120;
            int distFromBottom = videoPanel->ClientSize.Height - e->Y;
            controlPanel->Visible = distFromBottom <= threshold;
        }

        System::Void ControlPanel_Resize(System::Object^ sender, System::EventArgs^ e) {
            ArrangeControlsCentered();
        }

        System::Void ShuffleButton_Click(System::Object^ sender, System::EventArgs^ e) {
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

        void CaptureOriginalControlPositions() {
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
            this->AddPos(L"volumeTrackBar", this->volumeTrackBar);
            this->AddPos(L"speedComboBox", this->speedComboBox);
            this->AddPos(L"positionTrackBar", this->positionTrackBar);
            this->AddPos(L"timeLabel", this->timeLabel);
            this->AddPos(L"volumeIcon", this->volumeIcon);
            this->AddPos(L"speedIcon", this->speedIcon);
        }

        void RestoreOriginalControlPositions() {
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
            this->SetPos(L"volumeTrackBar", this->volumeTrackBar);
            this->SetPos(L"speedComboBox", this->speedComboBox);
            this->SetPos(L"positionTrackBar", this->positionTrackBar);
            this->SetPos(L"timeLabel", this->timeLabel);
            this->SetPos(L"volumeIcon", this->volumeIcon);
            this->SetPos(L"speedIcon", this->speedIcon);
        }

        void AddPos(System::String^ key, System::Windows::Forms::Control^ c) {
            if (c != nullptr && this->originalPositions != nullptr && !this->originalPositions->ContainsKey(key)) {
                this->originalPositions->Add(key, c->Location);
            }
        }

        void SetPos(System::String^ key, System::Windows::Forms::Control^ c) {
            if (c != nullptr && this->originalPositions != nullptr && this->originalPositions->ContainsKey(key)) {
                c->Location = this->originalPositions[key];
            }
        }
    };
}