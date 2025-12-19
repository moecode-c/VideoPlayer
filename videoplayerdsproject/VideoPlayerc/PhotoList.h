#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;

ref class PhotoList
{
public:
    ref class Node
    {
    public:
        String^ photoPath;
        String^ photoName;
        String^ albumName;
        bool isAlbumHeader;
        bool isExpanded;
        DateTime addedTime;
        Node^ next;
        Node(String^ path, String^ name, String^ album, bool isHeader);
    };

    // Simple array wrapper for search results (NO STL)
    ref class NodeArray
    {
    public:
        array<Node^>^ nodes;
        int count;

        NodeArray()
        {
            count = 0;
            nodes = gcnew array<Node^>(1000);
            for (int i = 0; i < 1000; i++)
                nodes[i] = nullptr;
        }

        void add(Node^ node)
        {
            if (count < nodes->Length)
            {
                nodes[count] = node;
                count++;
            }
        }

        Node^ get(int index)
        {
            if (index >= 0 && index < count)
                return nodes[index];
            return nullptr;
        }
    };

private:
    Node^ head;
    Node^ tail;
    Node^ current;
    int size;

    void appendNode(Node^ node);
    bool isDuplicate(String^ path);
    Node^ getPhotoAt(int index);
    NodeArray^ getAllPlayablePhotos();

public:
    PhotoList();

    // Basic operations
    bool isEmpty();
    int getSize();

    // Add operations
    void addAlbum(array<String^>^ photoPaths, String^ albumName, ListBox^ box);
    void addPhoto(String^ path, String^ name, ListBox^ box);

    // Album operations
    void toggleAlbumExpansion(int index, ListBox^ box);
    NodeArray^ getAlbumPhotos(String^ albumName);

    // Display operations
    void populatePhotoList(ListBox^ box);
    Node^ getNodeAtDisplayIndex(int displayIndex, ListBox^ box);

    // Navigation operations
    String^ nextPhoto();
    String^ previousPhoto();
    String^ getRandomPhoto();

    // Current node operations
    String^ getCurrentNodePath();
    String^ getCurrentNodeName();
    void setCurrentNode(Node^ node);
    void setCurrentNode(int index);
    Node^ getCurrentNode();
    int getCurrentNodeIndex();

    // Remove operations
    void removeItem(int displayIndex, ListBox^ box);
    void clearAll(ListBox^ box);

    // File operations
    void saveToFile(String^ filename, ListBox^ box);
    void loadFromFile(String^ filename, ListBox^ box);

    // Search operations
    NodeArray^ searchPhotos(String^ searchTerm);

    // Sorting operations
    void sortAlphabetically(ListBox^ box);
    void sortByTimeAdded(ListBox^ box);

    // Utility operations
    int shuffle();
    void printList();
};
