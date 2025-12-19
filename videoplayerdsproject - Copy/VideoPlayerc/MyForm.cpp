#include "MenuForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Start with the main menu
    VideoPlayerc::MenuForm^ menuForm = gcnew VideoPlayerc::MenuForm();
    Application::Run(menuForm);

    return 0;
}