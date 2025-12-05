#include "LinkedList.h"


// ------------------------------
// Node Constructor
// ------------------------------
VideoList::Node::Node(String^ path, String^ name)
{
    videoPath = path;
    videoName = name;
    next = nullptr;
}

// ------------------------------
// Constructor
// ------------------------------
VideoList::VideoList()
{
    head = nullptr;
    current = nullptr;
    size = 0;
}

// ------------------------------
bool VideoList::isEmpty() { return head == nullptr; }

// ------------------------------
// ADD VIDEO
// ------------------------------
void VideoList::addVideo(String^ path, String^ name, ListBox^ box)
{
    if (isDuplicate(path))
    {
        System::Windows::Forms::DialogResult result = MessageBox::Show(
            "This video already exists in the playlist!\n\nDo you want to add it anyway?",
            "Duplicate Video",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Warning
        );

        if (result == System::Windows::Forms::DialogResult::No)
            return;
    }

    Node^ newNode = gcnew Node(path, name);

    if (head == nullptr)
    {
        head = newNode;
        current = head;
    }
    else
    {
        Node^ temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = newNode;
    }

    size++;
    arrangeAlphabetically(box);
}

// ------------------------------
// NEXT VIDEO
// ------------------------------
String^ VideoList::nextVideo()
{
    if (isEmpty()) return nullptr;
    if (current->next == nullptr) return current->videoPath;
    current = current->next;
    return current->videoPath;
}

// ------------------------------
// PREVIOUS VIDEO
// ------------------------------
String^ VideoList::prevVideo()
{
    if (isEmpty()) return nullptr;
    current = head;
    return current->videoPath;
}

// ------------------------------
String^ VideoList::getCurrentNodePath() { return isEmpty() ? nullptr : current->videoPath; }
String^ VideoList::getCurrentNodeName() { return isEmpty() ? nullptr : current->videoName; }

// ------------------------------
// SET CURRENT BY INDEX
// ------------------------------
void VideoList::setCurrentNode(int index)
{
    if (index < 0 || index >= size) return;
    Node^ temp = head;
    for (int i = 0; i < index; i++)
        temp = temp->next;
    current = temp;
}

// ------------------------------
// GET NODE BY INDEX
// ------------------------------
VideoList::Node^ VideoList::getVideoAt(int index)
{
    if (index < 0 || index >= size) return nullptr;
    Node^ temp = head;
    for (int i = 0; i < index; i++)
        temp = temp->next;
    return temp;
}

// ------------------------------
int VideoList::getCurrentNodeIndex()
{
    Node^ temp = head;
    int idx = 0;
    while (temp != nullptr)
    {
        if (temp == current) return idx;
        temp = temp->next;
        idx++;
    }
    return -1;
}

// ------------------------------
// POPULATE LISTBOX
// ------------------------------
void VideoList::populateTrackList(ListBox^ box)
{
    if (box == nullptr) return;
    box->Items->Clear();
    Node^ temp = head;
    while (temp != nullptr)
    {
        box->Items->Add(temp->videoName);
        temp = temp->next;
    }
}

// ------------------------------
// REMOVE VIDEO
// ------------------------------
void VideoList::removeVideo(ListBox^ box)
{
    if (isEmpty() || box == nullptr) return;
    int index = box->SelectedIndex;
    if (index < 0 || index >= size) return;

    Node^ temp = head;
    Node^ prev = nullptr;

    for (int i = 0; i < index; i++)
    {
        prev = temp;
        temp = temp->next;
    }

    if (prev == nullptr)
        head = head->next;
    else
        prev->next = temp->next;

    if (current == temp)
        current = head;

    size--;
    populateTrackList(box);
}

// ------------------------------
// SHUFFLE
// ------------------------------
int VideoList::shuffle()
{
    if (isEmpty()) return -1;
    Random^ rand = gcnew Random();
    int index = rand->Next(0, size);
    setCurrentNode(index);
    return index;
}

// ------------------------------
// SAVE
// ------------------------------
void VideoList::saveToFile(String^ filename, ListBox^ box)
{
    try
    {
        StreamWriter^ sw = gcnew StreamWriter(filename);
        Node^ temp = head;
        while (temp != nullptr)
        {
            sw->WriteLine(temp->videoPath);
            temp = temp->next;
        }
        sw->Close();
    }
    catch (Exception^ ex)
    {
        MessageBox::Show("Error saving file: " + ex->Message);
    }
}

// ------------------------------
// LOAD
// ------------------------------
void VideoList::loadFromFile(String^ filename, ListBox^ box)
{
    try
    {
        StreamReader^ sr = gcnew StreamReader(filename);
        String^ line;
        while ((line = sr->ReadLine()) != nullptr)
        {
            String^ name = Path::GetFileName(line);
            addVideo(line, name, box);
        }
        sr->Close();
    }
    catch (Exception^ ex)
    {
        MessageBox::Show("Error loading file: " + ex->Message);
    }
}

// ------------------------------
// ARRANGE ALPHABETICALLY
// ------------------------------
void VideoList::arrangeAlphabetically(ListBox^ box)
{
    if (isEmpty() || size <= 1) return;

    bool swapped;
    do
    {
        swapped = false;
        Node^ curr = head;
        Node^ prev = nullptr;
        Node^ next = head->next;

        while (next != nullptr)
        {
            if (String::Compare(curr->videoName, next->videoName, true) > 0)
            {
                swapped = true;

                if (prev != nullptr)
                    prev->next = next;
                else
                    head = next;

                curr->next = next->next;
                next->next = curr;

                prev = next;
                next = curr->next;
            }
            else
            {
                prev = curr;
                curr = next;
                next = next->next;
            }
        }
    } while (swapped);

    current = head;

    if (box != nullptr)
        populateTrackList(box);
}

// ------------------------------
void VideoList::printList()
{
    Node^ temp = head;
    while (temp != nullptr)
    {
        Console::WriteLine(temp->videoName);
        temp = temp->next;
    }
}

int VideoList::getSize() { return size; }

// ------------------------------
// DUPLICATE CHECK
// ------------------------------
bool VideoList::isDuplicate(String^ path)
{
    Node^ temp = head;
    while (temp != nullptr)
    {
        if (String::Compare(temp->videoPath, path, true) == 0)
            return true;
        temp = temp->next;
    }
    return false;
}

