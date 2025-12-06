#include "MyForm.h"


using namespace VideoPlayerw;

[STAThreadAttribute]
int main(array<System::String^>^)
{
    Application::EnableVisualStyles();
    Application::Run(gcnew MainForm());
    return 0;
}