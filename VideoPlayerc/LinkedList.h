#pragma once
#include <msclr/marshal_cppstd.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;


ref class VideoList
{
public:
    ref class Node
    {
    public:
        String^ videoPath;
        String^ videoName;
        String^ playlistName;
        bool isPlaylistHeader;
        bool isExpanded;
        DateTime addedTime;
        Node^ next;

        Node(String^ path, String^ name, String^ playlist, bool isHeader);
    };

private:
    Node^ head;
    Node^ tail;
    Node^ current;
    int size;

    void appendNode(Node^ node);
    bool isDuplicate(String^ path);
    Node^ getVideoAt(int index);
    List<Node^>^ getAllPlayableVideos();

public:
    VideoList();

    // Basic operations
    bool isEmpty();
    int getSize();

    // Add operations
    void addPlaylist(array<String^>^ videoPaths, String^ playlistName, ListBox^ box);
    void addVideo(String^ path, String^ name, ListBox^ box);

    // Playlist operations
    void togglePlaylistExpansion(int index, ListBox^ box);
    List<Node^>^ getPlaylistVideos(String^ playlistName);

    // Display operations
    void populateTrackList(ListBox^ box);
    Node^ getNodeAtDisplayIndex(int displayIndex, ListBox^ box);

    // Playback operations
    String^ nextVideo();
    String^ previousVideo();
    String^ getRandomVideo();

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
    List<Node^>^ searchVideos(String^ searchTerm);

    // Sorting operations
    void sortAlphabetically(ListBox^ box);
    void sortByTimeAdded(ListBox^ box);

    // Utility operations
    int shuffle();
    void printList();
};