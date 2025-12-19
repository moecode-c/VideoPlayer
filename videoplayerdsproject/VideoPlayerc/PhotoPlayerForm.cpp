#include "PhotoPlayerForm.h"

using namespace VideoPlayerc;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::IO;

// Helper function to get user input (replaces InputBox)
String^ GetInputFromUser(String^ prompt, String^ title, String^ defaultValue)
{
    Form^ inputForm = gcnew Form();
    Label^ label = gcnew Label();
    TextBox^ textBox = gcnew TextBox();
    Button^ buttonOk = gcnew Button();
    Button^ buttonCancel = gcnew Button();

    inputForm->Text = title;
    inputForm->ClientSize = System::Drawing::Size(350, 120);
    inputForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
    inputForm->StartPosition = FormStartPosition::CenterScreen;
    inputForm->MinimizeBox = false;
    inputForm->MaximizeBox = false;

    label->Text = prompt;
    label->Location = System::Drawing::Point(10, 10);
    label->Size = System::Drawing::Size(330, 20);

    textBox->Text = defaultValue;
    textBox->Location = System::Drawing::Point(10, 35);
    textBox->Size = System::Drawing::Size(330, 25);

    buttonOk->Text = "OK";
    buttonOk->DialogResult = System::Windows::Forms::DialogResult::OK;
    buttonOk->Location = System::Drawing::Point(180, 70);
    buttonOk->Size = System::Drawing::Size(75, 30);

    buttonCancel->Text = "Cancel";
    buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    buttonCancel->Location = System::Drawing::Point(265, 70);
    buttonCancel->Size = System::Drawing::Size(75, 30);

    inputForm->Controls->Add(label);
    inputForm->Controls->Add(textBox);
    inputForm->Controls->Add(buttonOk);
    inputForm->Controls->Add(buttonCancel);
    inputForm->AcceptButton = buttonOk;
    inputForm->CancelButton = buttonCancel;

    if (inputForm->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        return textBox->Text;
    }

}

// Add Photos Button
System::Void PhotoPlayerForm::btnAddPhotos_Click(System::Object^ sender, System::EventArgs^ e)
{
    OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
    openFileDialog->Filter = "Image Files|*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff|All Files|*.*";
    openFileDialog->Multiselect = true;
    openFileDialog->Title = "Select Photos";

    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        for each (String ^ filePath in openFileDialog->FileNames)
        {
            String^ fileName = Path::GetFileName(filePath);
            photoList->addPhoto(filePath, fileName, listBoxPhotos);
        }

        if (photoList->getCurrentNodePath() != nullptr)
        {
            loadPhoto(photoList->getCurrentNodePath());
        }
    }
}

// Add Album Button
System::Void PhotoPlayerForm::btnAddAlbum_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ albumName = GetInputFromUser(
        "Enter album name:",
        "Create Album",
        "My Album"
    );

    if (String::IsNullOrWhiteSpace(albumName))
        return;

    OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
    openFileDialog->Filter = "Image Files|*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff|All Files|*.*";
    openFileDialog->Multiselect = true;
    openFileDialog->Title = "Select Photos for Album";

    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        photoList->addAlbum(openFileDialog->FileNames, albumName, listBoxPhotos);

        if (photoList->getCurrentNodePath() != nullptr)
        {
            loadPhoto(photoList->getCurrentNodePath());
        }
    }
}

// Remove Button
System::Void PhotoPlayerForm::btnRemove_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->SelectedIndex >= 0)
    {
        photoList->removeItem(listBoxPhotos->SelectedIndex, listBoxPhotos);

        if (!photoList->isEmpty() && photoList->getCurrentNodePath() != nullptr)
        {
            loadPhoto(photoList->getCurrentNodePath());
        }
        else
        {
            pictureBoxPhoto->Image = nullptr;
            lblPhotoName->Text = "Photo Player - No Photo Loaded";
            lblPhotoInfo->Text = "Load photos to get started";
        }
    }
    else
    {
        MessageBox::Show("Please select an item to remove.", "No Selection",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
}

// Clear All Button
System::Void PhotoPlayerForm::btnClear_Click(System::Object^ sender, System::EventArgs^ e)
{
    photoList->clearAll(listBoxPhotos);
    pictureBoxPhoto->Image = nullptr;
    lblPhotoName->Text = "Photo Player - No Photo Loaded";
    lblPhotoInfo->Text = "Load photos to get started";
    currentZoomLevel = 1.0f;
    trackBarZoom->Value = 100;
    lblZoom->Text = "Zoom: 100%";
}

// Save Button
System::Void PhotoPlayerForm::btnSave_Click(System::Object^ sender, System::EventArgs^ e)
{
    SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
    saveFileDialog->Filter = "Photo List Files (*.phl)|*.phl|All Files (*.*)|*.*";
    saveFileDialog->DefaultExt = "phl";
    saveFileDialog->Title = "Save Photo List";

    if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        photoList->saveToFile(saveFileDialog->FileName, listBoxPhotos);
    }
}

// Load Button
System::Void PhotoPlayerForm::btnLoad_Click(System::Object^ sender, System::EventArgs^ e)
{
    OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
    openFileDialog->Filter = "Photo List Files (*.phl)|*.phl|All Files (*.*)|*.*";
    openFileDialog->Title = "Load Photo List";

    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        photoList->loadFromFile(openFileDialog->FileName, listBoxPhotos);

        if (!photoList->isEmpty() && photoList->getCurrentNodePath() != nullptr)
        {
            loadPhoto(photoList->getCurrentNodePath());
        }
    }
}

// Sort A-Z Button
System::Void PhotoPlayerForm::btnSortAZ_Click(System::Object^ sender, System::EventArgs^ e)
{
    photoList->sortAlphabetically(listBoxPhotos);
}

// Sort by Time Button
System::Void PhotoPlayerForm::btnSortTime_Click(System::Object^ sender, System::EventArgs^ e)
{
    photoList->sortByTimeAdded(listBoxPhotos);
}

// Previous Button
System::Void PhotoPlayerForm::btnPrevious_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ prevPhotoPath = photoList->previousPhoto();
    if (prevPhotoPath != nullptr)
    {
        loadPhoto(prevPhotoPath);
        currentZoomLevel = 1.0f;
        trackBarZoom->Value = 100;
        lblZoom->Text = "Zoom: 100%";
    }
    else
    {
        MessageBox::Show("No previous photo available.", "Info",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
}

// Next Button
System::Void PhotoPlayerForm::btnNext_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ nextPhotoPath = photoList->nextPhoto();
    if (nextPhotoPath != nullptr)
    {
        loadPhoto(nextPhotoPath);
        currentZoomLevel = 1.0f;
        trackBarZoom->Value = 100;
        lblZoom->Text = "Zoom: 100%";
    }
    else
    {
        MessageBox::Show("No next photo available.", "Info",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
}

// Shuffle Button
System::Void PhotoPlayerForm::btnShuffle_Click(System::Object^ sender, System::EventArgs^ e)
{
    int index = photoList->shuffle();
    if (index >= 0)
    {
        String^ photoPath = photoList->getCurrentNodePath();
        if (photoPath != nullptr)
        {
            loadPhoto(photoPath);
            currentZoomLevel = 1.0f;
            trackBarZoom->Value = 100;
            lblZoom->Text = "Zoom: 100%";
        }
    }
    else
    {
        MessageBox::Show("No photos available to shuffle.", "Info",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
}

// Zoom In Button
System::Void PhotoPlayerForm::btnZoomIn_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (pictureBoxPhoto->Image != nullptr)
    {
        currentZoomLevel += 0.1f;
        if (currentZoomLevel > 4.0f) currentZoomLevel = 4.0f;
        trackBarZoom->Value = (int)(currentZoomLevel * 100);
        applyZoom(currentZoomLevel);
    }
}

// Zoom Out Button
System::Void PhotoPlayerForm::btnZoomOut_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (pictureBoxPhoto->Image != nullptr)
    {
        currentZoomLevel -= 0.1f;
        if (currentZoomLevel < 0.1f) currentZoomLevel = 0.1f;
        trackBarZoom->Value = (int)(currentZoomLevel * 100);
        applyZoom(currentZoomLevel);
    }
}

// Zoom Reset Button
System::Void PhotoPlayerForm::btnZoomReset_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (pictureBoxPhoto->Image != nullptr)
    {
        currentZoomLevel = 1.0f;
        trackBarZoom->Value = 100;
        pictureBoxPhoto->SizeMode = PictureBoxSizeMode::Zoom;
        applyZoom(currentZoomLevel);
    }
}

// Rotate Left Button
System::Void PhotoPlayerForm::btnRotateLeft_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (pictureBoxPhoto->Image != nullptr)
    {
        pictureBoxPhoto->Image->RotateFlip(RotateFlipType::Rotate270FlipNone);
        pictureBoxPhoto->Refresh();
    }
}

// Rotate Right Button
System::Void PhotoPlayerForm::btnRotateRight_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (pictureBoxPhoto->Image != nullptr)
    {
        pictureBoxPhoto->Image->RotateFlip(RotateFlipType::Rotate90FlipNone);
        pictureBoxPhoto->Refresh();
    }
}

// ListBox Double Click
System::Void PhotoPlayerForm::listBoxPhotos_DoubleClick(System::Object^ sender, System::EventArgs^ e)
{
    if (listBoxPhotos->SelectedIndex >= 0)
    {
        PhotoList::Node^ selectedNode = photoList->getNodeAtDisplayIndex(
            listBoxPhotos->SelectedIndex, listBoxPhotos);

        if (selectedNode != nullptr)
        {
            if (selectedNode->isAlbumHeader)
            {
                photoList->toggleAlbumExpansion(listBoxPhotos->SelectedIndex, listBoxPhotos);
            }
            else if (selectedNode->photoPath != nullptr)
            {
                photoList->setCurrentNode(selectedNode);
                loadPhoto(selectedNode->photoPath);
                currentZoomLevel = 1.0f;
                trackBarZoom->Value = 100;
                lblZoom->Text = "Zoom: 100%";
            }
        }
    }
}

// Search TextBox Enter
System::Void PhotoPlayerForm::txtSearch_Enter(System::Object^ sender, System::EventArgs^ e)
{
    if (txtSearch->Text->Length == 0)
    {
        txtSearch->ForeColor = Color::White;
    }
}

// Search TextBox Leave
System::Void PhotoPlayerForm::txtSearch_Leave(System::Object^ sender, System::EventArgs^ e)
{
    if (String::IsNullOrWhiteSpace(txtSearch->Text))
    {
        txtSearch->ForeColor = Color::Gray;
    }
}

// Search TextBox Text Changed
System::Void PhotoPlayerForm::txtSearch_TextChanged(System::Object^ sender, System::EventArgs^ e)
{
    if (txtSearch->Text != "🔍 Search photos..." && !String::IsNullOrWhiteSpace(txtSearch->Text))
    {
        PhotoList::NodeArray^ results = photoList->searchPhotos(txtSearch->Text);

        listBoxPhotos->Items->Clear();
        for (int i = 0; i < results->count; i++)
        {
            PhotoList::Node^ node = results->get(i);
            if (node != nullptr)
            {
                listBoxPhotos->Items->Add("* " + node->photoName);
            }
        }
    }
    else
    {
        photoList->populatePhotoList(listBoxPhotos);
    }
}

// TrackBar Zoom Scroll
System::Void PhotoPlayerForm::trackBarZoom_Scroll(System::Object^ sender, System::EventArgs^ e)
{
    if (pictureBoxPhoto->Image != nullptr)
    {
        currentZoomLevel = trackBarZoom->Value / 100.0f;
        applyZoom(currentZoomLevel);
    }
}

// Search Button Click
System::Void PhotoPlayerForm::btnSearch_Click(System::Object^ sender, System::EventArgs^ e)
{
    performSearch();
}

// performSearch helper
void PhotoPlayerForm::performSearch()
{
    try {
        String^ term = this->txtSearch->Text;
        if (String::IsNullOrWhiteSpace(term)) return;
        PhotoList::NodeArray^ results = this->photoList->searchPhotos(term);
        if (results == nullptr || results->count == 0) {
            MessageBox::Show("No photos found.", "Search", MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }
        PhotoList::Node^ first = results->get(0);
        if (first == nullptr) return;
        this->photoList->setCurrentNode(first);
        this->photoList->populatePhotoList(this->listBoxPhotos);
        for (int i = 0; i < this->listBoxPhotos->Items->Count; i++) {
            PhotoList::Node^ n = this->photoList->getNodeAtDisplayIndex(i, this->listBoxPhotos);
            if (n == first) { this->listBoxPhotos->SelectedIndex = i; break; }
        }
    }
    catch (...) {}
}

// PictureBox Mouse Down
System::Void PhotoPlayerForm::pictureBoxPhoto_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
    if (e->Button == System::Windows::Forms::MouseButtons::Left && pictureBoxPhoto->Image != nullptr)
    {
        isDragging = true;
        lastMousePos = e->Location;
        pictureBoxPhoto->Cursor = Cursors::Hand;
    }
}

// PictureBox Mouse Move
System::Void PhotoPlayerForm::pictureBoxPhoto_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
    if (isDragging && pictureBoxPhoto->Image != nullptr)
    {
        int deltaX = e->X - lastMousePos.X;
        int deltaY = e->Y - lastMousePos.Y;
        imageOffset.X += deltaX;
        imageOffset.Y += deltaY;
        lastMousePos = e->Location;
        pictureBoxPhoto->Refresh();
    }
}

// PictureBox Mouse Up
System::Void PhotoPlayerForm::pictureBoxPhoto_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
    isDragging = false;
    pictureBoxPhoto->Cursor = Cursors::Default;
}

// Slideshow button - placeholder implementation
System::Void PhotoPlayerForm::btnSlideshow_Click(System::Object^ sender, System::EventArgs^ e)
{
    // Placeholder: toggle a simple slideshow message for now.
    MessageBox::Show("Slideshow button clicked. Implement slideshow logic in PhotoPlayerForm.cpp.", "Slideshow", MessageBoxButtons::OK, MessageBoxIcon::Information);
}

// Speed button - placeholder implementation
System::Void PhotoPlayerForm::btnSpeed_Click(System::Object^ sender, System::EventArgs^ e)
{
    // Placeholder: show message. Could open speed selection UI.
    MessageBox::Show("Speed button clicked. Implement speed control logic in PhotoPlayerForm.cpp.", "Speed", MessageBoxButtons::OK, MessageBoxIcon::Information);
}

// Helper Method: Load Photo
void PhotoPlayerForm::loadPhoto(String^ photoPath)
{
    try
    {
        if (File::Exists(photoPath))
        {
            // Load image
            Image^ img = Image::FromFile(photoPath);
            pictureBoxPhoto->Image = img;
            pictureBoxPhoto->SizeMode = PictureBoxSizeMode::Zoom;

            // Update UI
            updatePhotoInfo();
            currentZoomLevel = 1.0f;
            trackBarZoom->Value = 100;
            lblZoom->Text = "Zoom: 100%";
            imageOffset = Point(0, 0);
        }
        else
        {
            MessageBox::Show("Photo file not found: " + photoPath, "Error",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
    catch (Exception^ ex)
    {
        MessageBox::Show("Error loading photo: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// Helper Method: Update Photo Info
void PhotoPlayerForm::updatePhotoInfo()
{
    String^ photoName = photoList->getCurrentNodeName();
    String^ photoPath = photoList->getCurrentNodePath();

    if (photoName != nullptr && photoPath != nullptr)
    {
        lblPhotoName->Text = photoName;

        try
        {
            FileInfo^ fileInfo = gcnew FileInfo(photoPath);
            double fileSizeKB = fileInfo->Length / 1024.0;
            String^ fileSize = fileSizeKB < 1024 ?
                String::Format("{0:F2} KB", fileSizeKB) :
                String::Format("{0:F2} MB", fileSizeKB / 1024.0);

            Image^ img = pictureBoxPhoto->Image;
            String^ dimensions = String::Format("{0} x {1} pixels", img->Width, img->Height);

            lblPhotoInfo->Text = String::Format("{0} | {1}", dimensions, fileSize);
        }
        catch (...)
        {
            lblPhotoInfo->Text = "Photo information unavailable";
        }
    }
}

// Helper Method: Apply Zoom
void PhotoPlayerForm::applyZoom(float zoomLevel)
{
    if (pictureBoxPhoto->Image != nullptr)
    {
        int newWidth = (int)(pictureBoxPhoto->Image->Width * zoomLevel);
        int newHeight = (int)(pictureBoxPhoto->Image->Height * zoomLevel);

        lblZoom->Text = String::Format("Zoom: {0}%", (int)(zoomLevel * 100));

        if (zoomLevel == 1.0f)
        {
            pictureBoxPhoto->SizeMode = PictureBoxSizeMode::Zoom;
        }
        else
        {
            pictureBoxPhoto->SizeMode = PictureBoxSizeMode::CenterImage;
        }

        pictureBoxPhoto->Refresh();
    }
}