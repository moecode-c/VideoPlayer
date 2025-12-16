#pragma once
using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::Drawing;

ref class PhotoList
{
private:
    ref struct Node
    {
        String^ photoPath;
        String^ photoName;
        Node^ next;
        DateTime addedTime;
        Node(String^ path, String^ name);
    };
    Node^ head;
    Node^ current;
    int size;

public:
    PhotoList();
    bool isEmpty();
    void addPhoto(String^ path, String^ name, ListBox^ box);
    String^ nextPhoto();
    String^ prevPhoto();
    String^ getCurrentNodePath();
    String^ getCurrentNodeName();
    void setCurrentNode(int index);
    void populatePhotoList(ListBox^ box);
    void removePhoto(ListBox^ box);
    int getSize();
    bool isDuplicate(String^ path);
};