#include "MyForm.h"
#include "MenuForm.h"


using namespace VideoPlayerc;

[STAThreadAttribute]
int main(array<System::String^>^)
{
    Application::EnableVisualStyles();
    // Start application with the MenuForm so user chooses Video or Photo player first
    Application::Run(gcnew MenuForm());
    return 0;
}
