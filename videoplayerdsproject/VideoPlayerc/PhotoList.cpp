#include "PhotoList.h"

PhotoList::Node::Node(String^ path, String^ name)
{
    photoPath = path;
    photoName = name;
    next = nullptr;
    addedTime = DateTime::Now;
}

PhotoList::PhotoList()
{
    head = nullptr;
    current = nullptr;
    size = 0;
}

bool PhotoList::isEmpty() { return head == nullptr; }

void PhotoList::addPhoto(String^ path, String^ name, ListBox^ box)
{
    if (isDuplicate(path))
    {
        System::Windows::Forms::DialogResult result = MessageBox::Show(
            "This photo already exists!\n\nAdd anyway?",
            "Duplicate Photo",
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
    if (box != nullptr)
        populatePhotoList(box);
}

String^ PhotoList::nextPhoto()
{
    if (isEmpty()) return nullptr;
    if (current->next == nullptr) return current->photoPath;
    current = current->next;
    return current->photoPath;
}

String^ PhotoList::prevPhoto()
{
    if (isEmpty()) return nullptr;
    if (current == head) return current->photoPath;

    Node^ temp = head;
    while (temp->next != current)
        temp = temp->next;
    current = temp;
    return current->photoPath;
}

String^ PhotoList::getCurrentNodePath() { return isEmpty() ? nullptr : current->photoPath; }
String^ PhotoList::getCurrentNodeName() { return isEmpty() ? nullptr : current->photoName; }

void PhotoList::setCurrentNode(int index)
{
    if (index < 0 || index >= size) return;
    Node^ temp = head;
    for (int i = 0; i < index; i++)
        temp = temp->next;
    current = temp;
}

void PhotoList::populatePhotoList(ListBox^ box)
{
    if (box == nullptr) return;
    box->Items->Clear();
    Node^ temp = head;
    while (temp != nullptr)
    {
        box->Items->Add(temp->photoName);
        temp = temp->next;
    }
}

void PhotoList::removePhoto(ListBox^ box)
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
    populatePhotoList(box);
}

int PhotoList::getSize() { return size; }

bool PhotoList::isDuplicate(String^ path)
{
    Node^ temp = head;
    while (temp != nullptr)
    {
        if (String::Compare(temp->photoPath, path, true) == 0)
            return true;
        temp = temp->next;
    }
    return false;
}