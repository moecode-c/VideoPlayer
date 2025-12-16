#include "MenuForm.h"
#include "MyForm.h"  // Video Player - UNCHANGED
#include "PhotoPlayerForm.h"  // Photo Player - NEW

using namespace VideoPlayerc;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;

System::Void MenuForm::btnVideoPlayer_Click(System::Object^ sender, System::EventArgs^ e)
{
    // Hide menu and show video player - UNCHANGED
    this->Hide();
    VideoPlayerc::MainForm^ videoForm = gcnew VideoPlayerc::MainForm();
    videoForm->FormClosed += gcnew FormClosedEventHandler(this, &MenuForm::OnChildFormClosed);
    videoForm->Show();
}

System::Void MenuForm::btnPhotoPlayer_Click(System::Object^ sender, System::EventArgs^ e)
{
    // Hide menu and show photo player
    this->Hide();
    VideoPlayerc::PhotoPlayerForm^ photoForm = gcnew VideoPlayerc::PhotoPlayerForm();
    photoForm->FormClosed += gcnew FormClosedEventHandler(this, &MenuForm::OnChildFormClosed);
    photoForm->Show();
}

void MenuForm::OnChildFormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
    // Show menu again when child form closes
    this->Show();
}