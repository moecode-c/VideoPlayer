#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

namespace VideoPlayerc {

    ref class PhotoList {
public:
    ref class Node {
    public:
        String^ photoName;
        String^ photoPath;
        DateTime addedTime;
        String^ albumName;
        bool isAlbumHeader;
        bool isExpanded;
    };

    ref class NodeArray {
    public:
        List<Node^>^ items;
        NodeArray() { items = gcnew List<Node^>(); }
        property int count { int get() { return items->Count; } }
        Node^ get(int i) { return (i >= 0 && i < items->Count) ? items[i] : nullptr; }
    };

    PhotoList();

    void populatePhotoList(ListBox^ lb);
    Node^ getCurrentNode();
    String^ getCurrentNodePath();
    String^ getCurrentNodeName();
    void addPhoto(String^ path, String^ name, ListBox^ lb);
    void addAlbum(array<String^>^ paths, String^ albumName, ListBox^ lb);
    void removeItem(int displayIndex, ListBox^ lb);
    void clearAll(ListBox^ lb);
    void saveToFile(String^ filename, ListBox^ lb);
    void loadFromFile(String^ filename, ListBox^ lb);
    void sortAlphabetically(ListBox^ lb);
    void sortByTimeAdded(ListBox^ lb);
    String^ previousPhoto();
    String^ nextPhoto();
    int shuffle();
    bool isEmpty();
    void toggleAlbumExpansion(int displayIndex, ListBox^ lb);
    void setCurrentNode(Node^ node);
    Node^ getNodeAtDisplayIndex(int displayIndex, ListBox^ lb);
    NodeArray^ searchPhotos(String^ term);

    private:
        List<Node^>^ nodes;
        // track currently selected photo without reordering the list
        Node^ currentNode;
    };

}
