#include "MyForm.h"


using namespace VideoPlayerc;

[STAThreadAttribute]
int main(array<System::String^>^)
{
    Application::EnableVisualStyles();
    Application::Run(gcnew MainForm());
    return 0;
}