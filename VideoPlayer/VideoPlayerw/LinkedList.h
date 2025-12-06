#pragma once
using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::Collections::Generic;

ref class VideoList
{
private:
    ref struct Node
    {
        String^ videoPath;
        String^ videoName;
        Node^ next;
        DateTime addedTime;  // ADD THIS LINE - to track when video was added
        Node(String^ path, String^ name);
    };
    Node^ head;
    Node^ current;
    int size;
public:
    VideoList();
    bool isEmpty();
    void addVideo(String^ path, String^ name, ListBox^ box);
    String^ nextVideo();
    String^ prevVideo();
    String^ getCurrentNodePath();
    String^ getCurrentNodeName();
    void setCurrentNode(int index);
    Node^ getVideoAt(int index);
    int getCurrentNodeIndex();
    void populateTrackList(ListBox^ box);
    void removeVideo(ListBox^ box);
    int shuffle();
    void saveToFile(String^ filename, ListBox^ box);
    void loadFromFile(String^ filename, ListBox^ box);
    void arrangeAlphabetically(ListBox^ box);
    void sortAlphabetically(ListBox^ box);  // ADD THIS LINE - for Sort A-Z button
    void sortByTime(ListBox^ box);          // ADD THIS LINE - for Sort by Time button
    void clearAll(ListBox^ box);            // ADD THIS LINE - for Clear All button
    void printList();
    int getSize();
    bool isDuplicate(String^ path);
};