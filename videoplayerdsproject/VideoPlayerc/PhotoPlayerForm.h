#pragma once
using namespace System;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace PhotoPlayer {
    ref class PhotoNode {
    public:
        String^ photoPath;
        PhotoNode^ next;
        PhotoNode(String^ path) {
            photoPath = path;
            next = nullptr;
        }
    };
    public ref class PhotoLinkedList {
    private:
        PhotoNode^ head;
        int count;
        

    public:
        PhotoLinkedList() {
            head = nullptr;
            count = 0;
        }
        void AddPhoto(String^ path) {
            PhotoNode^ newNode = gcnew PhotoNode(path);
            if (head == nullptr) {
                head = newNode;
            }
            else {
                PhotoNode^ current = head;
                while (current->next != nullptr)
                    current = current->next;
                current->next = newNode;
            }
            count++;
        }
        String^ GetPhotoAt(int index) {
            if (index < 0 || index >= count)
                return nullptr;
            PhotoNode^ current = head;
            for (int i = 0; i < index; i++)
                current = current->next;
            return current->photoPath;
        }
        bool DeletePhotoAt(int index) {
            if (index < 0 || index >= count)
                return false;
            if (index == 0) {
                PhotoNode^ temp = head;
                head = head->next;
                delete temp;
                count--;
                return true;
            }
            PhotoNode^ current = head;
            for (int i = 0; i < index - 1; i++)
                current = current->next;
            PhotoNode^ temp = current->next;
            current->next = temp->next;
            delete temp;
            count--;
            return true;
        }
        void Reverse() {
            PhotoNode^ prev = nullptr;
            PhotoNode^ current = head;
            PhotoNode^ next = nullptr;
            while (current != nullptr) {
                next = current->next;
                current->next = prev;
                prev = current;
                current = next;
            }
            head = prev;
        }
        property int Count {
            int get() { return count; }
        }


    };
}

namespace VideoPlayerc {
    public ref class PhotoPlayerForm : public System::Windows::Forms::Form
    {
    public:
        PhotoPlayerForm(void);

    protected:
        ~PhotoPlayerForm();

    private:
        System::ComponentModel::Container^ components;
        PhotoPlayer::PhotoLinkedList^ photoList;

        PictureBox^ pictureBox;
        Button^ btnUpload;
        Button^ btnDelete;
        Button^ btnPrevious;
        Button^ btnNext;
        Button^ btnFirst;
        Button^ btnLast;
        Button^ btnRotateLeft;
        Button^ btnRotateRight;
        Button^ btnSlideshow;
        Button^ btnZoomIn;
        Button^ btnZoomOut;
        Button^ btnResetZoom;
        Button^ btnSortByName;
        Button^ btnReverse;
        Button^ btnBackToMenu;
        ListBox^ listBoxPhotos;
        Label^ lblInfo;
        Label^ lblCurrentPhoto;
        TextBox^ txtSearch;
        Button^ btnSearch;
        Button^ btnClearSearch;
        TrackBar^ trackBarSpeed;
        Label^ lblSpeed;
        System::Drawing::Bitmap^ originalImage;
        System::Drawing::Bitmap^ currentImage;
        Timer^ slideshowTimer;
        bool isSlideshowRunning;
        float currentZoom;
        int currentRotation;

        // ***************** MEDIA CONTROL - PANNING *****************
        bool isPanning;
        System::Drawing::Point panStartPoint;
        System::Drawing::Point imageLocation;
        // ***************** MEDIA CONTROL - PANNING *****************

        void InitializeComponent(void);
        System::Void btnUpload_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void listBoxPhotos_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnPrevious_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnNext_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnFirst_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnLast_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnRotateLeft_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnRotateRight_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSlideshow_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void slideshowTimer_Tick(System::Object^ sender, System::EventArgs^ e);
        System::Void btnZoomIn_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnZoomOut_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnResetZoom_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSortByName_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnReverse_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSearch_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnClearSearch_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void trackBarSpeed_Scroll(System::Object^ sender, System::EventArgs^ e);
        System::Void btnBackToMenu_Click(System::Object^ sender, System::EventArgs^ e);

		// ***************** MEDIA CONTROL - PANNING *****************
        //System::Void btnBackToMenu_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void pictureBox_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void pictureBox_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void pictureBox_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        // ***************** MEDIA CONTROL - PANNING *****************

        void DisplayPhoto(int index);
        void UpdateNavigationButtons();
        void UpdatePhotoInfo();
        void ApplyZoom();


    };
}