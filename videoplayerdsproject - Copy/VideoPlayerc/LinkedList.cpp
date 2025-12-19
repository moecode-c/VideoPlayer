#include "LinkedList.h"

// Node Constructor
VideoList::Node::Node(String^ path, String^ name, String^ playlist, bool isHeader)
{
    videoPath = path;
    videoName = name;
    playlistName = playlist;
    isPlaylistHeader = isHeader;
    isExpanded = false;
    addedTime = DateTime::Now;
    next = nullptr;
}

// VideoList Constructor
VideoList::VideoList()
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    size = 0;
}

// Private helper methods
void VideoList::appendNode(Node^ node)
{
    if (head == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail->next = node;
        tail = node;
    }
    size++;
}

bool VideoList::isDuplicate(String^ path)
{
    Node^ temp = head;
    while (temp != nullptr)
    {
        if (!temp->isPlaylistHeader &&
            String::Compare(temp->videoPath, path, true) == 0)
            return true;
        temp = temp->next;
    }
    return false;
}

VideoList::Node^ VideoList::getVideoAt(int index)
{
    if (index < 0 || index >= size) return nullptr;

    Node^ temp = head;
    for (int i = 0; i < index; i++)
    {
        if (temp == nullptr) return nullptr;
        temp = temp->next;
    }
    return temp;
}

List<VideoList::Node^>^ VideoList::getAllPlayableVideos()
{
    List<Node^>^ playableVideos = gcnew List<Node^>();
    Node^ temp = head;

    while (temp != nullptr)
    {
        if (!temp->isPlaylistHeader && temp->videoPath != nullptr)
            playableVideos->Add(temp);
        temp = temp->next;
    }

    return playableVideos;
}

// Basic operations
bool VideoList::isEmpty()
{
    return head == nullptr;
}

int VideoList::getSize()
{
    return size;
}

// Add operations
void VideoList::addPlaylist(array<String^>^ videoPaths, String^ playlistName, ListBox^ box)
{
    if (videoPaths == nullptr || videoPaths->Length == 0)
    {
        MessageBox::Show("No videos selected for playlist.", "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    Node^ headerNode = gcnew Node(nullptr, playlistName, playlistName, true);
    appendNode(headerNode);

    for each (String ^ path in videoPaths)
    {
        if (File::Exists(path))
        {
            String^ name = Path::GetFileName(path);
            Node^ videoNode = gcnew Node(path, name, playlistName, false);
            appendNode(videoNode);
        }
    }

    if (box != nullptr)
        populateTrackList(box);
}

void VideoList::addVideo(String^ path, String^ name, ListBox^ box)
{
    if (!File::Exists(path))
    {
        MessageBox::Show("Video file does not exist!", "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

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

    Node^ newNode = gcnew Node(path, name, nullptr, false);
    appendNode(newNode);

    if (current == nullptr)
        current = newNode;

    if (box != nullptr)
        populateTrackList(box);
}

// Playback operations
String^ VideoList::nextVideo()
{
    if (isEmpty() || current == nullptr) return nullptr;

    if (current->playlistName != nullptr && !current->isPlaylistHeader)
    {
        Node^ temp = current->next;
        while (temp != nullptr)
        {
            if (!temp->isPlaylistHeader && temp->playlistName == current->playlistName)
            {
                current = temp;
                return current->videoPath;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    Node^ temp = current->next;
    while (temp != nullptr)
    {
        if (!temp->isPlaylistHeader && temp->videoPath != nullptr)
        {
            current = temp;
            return current->videoPath;
        }
        temp = temp->next;
    }

    return nullptr;
}

String^ VideoList::previousVideo()
{
    if (isEmpty() || current == nullptr) return nullptr;

    Node^ temp = head;
    Node^ prev = nullptr;

    while (temp != nullptr && temp != current)
    {
        if (!temp->isPlaylistHeader && temp->videoPath != nullptr)
        {
            if (current->playlistName != nullptr)
            {
                if (temp->playlistName == current->playlistName)
                    prev = temp;
            }
            else
            {
                prev = temp;
            }
        }
        temp = temp->next;
    }

    if (prev != nullptr)
    {
        current = prev;
        return current->videoPath;
    }

    return nullptr;
}

String^ VideoList::getRandomVideo()
{
    if (isEmpty()) return nullptr;

    List<Node^>^ playableVideos = getAllPlayableVideos();

    if (playableVideos->Count == 0) return nullptr;

    Random^ rnd = gcnew Random();
    int index = rnd->Next(playableVideos->Count);
    current = playableVideos[index];
    return current->videoPath;
}

// Current node operations
String^ VideoList::getCurrentNodePath()
{
    return (isEmpty() || current == nullptr) ? nullptr : current->videoPath;
}

String^ VideoList::getCurrentNodeName()
{
    return (isEmpty() || current == nullptr) ? nullptr : current->videoName;
}

void VideoList::setCurrentNode(Node^ node)
{
    current = node;
}

void VideoList::setCurrentNode(int index)
{
    Node^ node = getVideoAt(index);
    if (node != nullptr)
        current = node;
}

VideoList::Node^ VideoList::getCurrentNode()
{
    return current;
}

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

// Display operations
void VideoList::populateTrackList(ListBox^ box)
{
    if (box == nullptr) return;

    box->BeginUpdate();
    box->Items->Clear();

    Node^ temp = head;
    while (temp != nullptr)
    {
        if (temp->isPlaylistHeader)
        {
            String^ icon = temp->isExpanded ? "[+] " : "[-] ";
            box->Items->Add(icon + temp->videoName);

            if (temp->isExpanded)
            {
                Node^ videoNode = temp->next;
                while (videoNode != nullptr && !videoNode->isPlaylistHeader)
                {
                    if (videoNode->playlistName == temp->playlistName)
                    {
                        Node^ nextNode = videoNode->next;
                        bool isLast = (nextNode == nullptr || nextNode->isPlaylistHeader ||
                            nextNode->playlistName != temp->playlistName);

                        String^ branch = isLast ? "  |__ " : "  |-- ";
                        box->Items->Add(branch + videoNode->videoName);
                    }
                    videoNode = videoNode->next;
                }
            }
        }
        else if (temp->playlistName == nullptr)
        {
            box->Items->Add("* " + temp->videoName);
        }

        temp = temp->next;
    }

    box->EndUpdate();
}

VideoList::Node^ VideoList::getNodeAtDisplayIndex(int displayIndex, ListBox^ box)
{
    int currentDisplayIndex = 0;
    Node^ temp = head;

    while (temp != nullptr)
    {
        if (temp->isPlaylistHeader)
        {
            if (currentDisplayIndex == displayIndex)
                return temp;
            currentDisplayIndex++;

            if (temp->isExpanded)
            {
                Node^ videoNode = temp->next;
                while (videoNode != nullptr && !videoNode->isPlaylistHeader)
                {
                    if (videoNode->playlistName == temp->playlistName)
                    {
                        if (currentDisplayIndex == displayIndex)
                            return videoNode;
                        currentDisplayIndex++;
                    }
                    videoNode = videoNode->next;
                }
            }
        }
        else if (temp->playlistName == nullptr)
        {
            if (currentDisplayIndex == displayIndex)
                return temp;
            currentDisplayIndex++;
        }

        temp = temp->next;
    }

    return nullptr;
}

// Playlist operations
void VideoList::togglePlaylistExpansion(int index, ListBox^ box)
{
    Node^ node = getNodeAtDisplayIndex(index, box);
    if (node != nullptr && node->isPlaylistHeader)
    {
        node->isExpanded = !node->isExpanded;
        populateTrackList(box);
    }
}

List<VideoList::Node^>^ VideoList::getPlaylistVideos(String^ playlistName)
{
    List<Node^>^ videos = gcnew List<Node^>();
    Node^ temp = head;

    while (temp != nullptr)
    {
        if (!temp->isPlaylistHeader && temp->playlistName == playlistName)
        {
            videos->Add(temp);
        }
        temp = temp->next;
    }

    return videos;
}

// Remove operations
void VideoList::removeItem(int displayIndex, ListBox^ box)
{
    Node^ nodeToRemove = getNodeAtDisplayIndex(displayIndex, box);
    if (nodeToRemove == nullptr) return;

    if (nodeToRemove->isPlaylistHeader)
    {
        System::Windows::Forms::DialogResult result = MessageBox::Show(
            "Delete playlist '" + nodeToRemove->videoName + "' and all its videos?",
            "Confirm Delete",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Question
        );

        if (result == System::Windows::Forms::DialogResult::No)
            return;

        String^ playlistToRemove = nodeToRemove->playlistName;

        Node^ temp = head;
        Node^ prev = nullptr;

        while (temp != nullptr)
        {
            Node^ nextNode = temp->next;

            if ((temp->isPlaylistHeader && temp->playlistName == playlistToRemove) ||
                (!temp->isPlaylistHeader && temp->playlistName == playlistToRemove))
            {
                if (prev == nullptr)
                {
                    head = nextNode;
                }
                else
                {
                    prev->next = nextNode;
                }

                if (temp == tail)
                {
                    tail = prev;
                }

                if (current == temp)
                    current = head;

                size--;
            }
            else
            {
                prev = temp;
            }

            temp = nextNode;
        }
    }
    else
    {
        Node^ temp = head;
        Node^ prev = nullptr;

        while (temp != nullptr)
        {
            if (temp == nodeToRemove)
            {
                if (prev == nullptr)
                {
                    head = head->next;
                    if (head == nullptr)
                        tail = nullptr;
                }
                else
                {
                    prev->next = temp->next;
                    if (temp == tail)
                        tail = prev;
                }

                if (current == temp)
                    current = head;

                size--;
                break;
            }
            prev = temp;
            temp = temp->next;
        }
    }

    populateTrackList(box);
}

void VideoList::clearAll(ListBox^ box)
{
    if (isEmpty()) return;

    System::Windows::Forms::DialogResult result = MessageBox::Show(
        "Are you sure you want to remove all playlists and videos?",
        "Clear All",
        MessageBoxButtons::YesNo,
        MessageBoxIcon::Question
    );

    if (result == System::Windows::Forms::DialogResult::No)
        return;

    head = nullptr;
    tail = nullptr;
    current = nullptr;
    size = 0;

    if (box != nullptr)
    {
        box->Items->Clear();
        box->Refresh();
    }

    MessageBox::Show("All items cleared.", "Cleared",
        MessageBoxButtons::OK, MessageBoxIcon::Information);
}

// File operations
void VideoList::saveToFile(String^ filename, ListBox^ box)
{
    try
    {
        StreamWriter^ sw = gcnew StreamWriter(filename);
        sw->WriteLine("# VideoList Save File v1.0");
        sw->WriteLine("# Generated: " + DateTime::Now.ToString());

        Node^ temp = head;

        while (temp != nullptr)
        {
            if (temp->isPlaylistHeader)
            {
                sw->WriteLine("[PLAYLIST]" + temp->playlistName);
            }
            else if (temp->playlistName != nullptr)
            {
                sw->WriteLine("[PLAYLISTVIDEO]" + temp->playlistName + "|" + temp->videoPath);
            }
            else
            {
                sw->WriteLine("[VIDEO]" + temp->videoPath);
            }
            temp = temp->next;
        }

        sw->Close();

    }
    catch (Exception^ ex)
    {
        MessageBox::Show("Error saving file: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void VideoList::loadFromFile(String^ filename, ListBox^ box)
{
    try
    {
        if (!File::Exists(filename))
        {
            MessageBox::Show("File does not exist!", "Error",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        StreamReader^ sr = gcnew StreamReader(filename);
        String^ line;

        while ((line = sr->ReadLine()) != nullptr)
        {
            if (line->StartsWith("#") || String::IsNullOrWhiteSpace(line))
                continue;

            if (line->StartsWith("[PLAYLIST]"))
            {
                String^ playlistName = line->Substring(10);
                Node^ headerNode = gcnew Node(nullptr, playlistName, playlistName, true);
                appendNode(headerNode);
            }
            else if (line->StartsWith("[PLAYLISTVIDEO]"))
            {
                String^ data = line->Substring(15);
                int separatorIndex = data->IndexOf('|');

                if (separatorIndex > 0)
                {
                    String^ playlistName = data->Substring(0, separatorIndex);
                    String^ videoPath = data->Substring(separatorIndex + 1);

                    if (File::Exists(videoPath))
                    {
                        String^ videoName = Path::GetFileName(videoPath);
                        Node^ videoNode = gcnew Node(videoPath, videoName, playlistName, false);
                        appendNode(videoNode);
                    }
                }
            }
            else if (line->StartsWith("[VIDEO]"))
            {
                String^ videoPath = line->Substring(7);

                if (File::Exists(videoPath))
                {
                    String^ videoName = Path::GetFileName(videoPath);
                    addVideo(videoPath, videoName, nullptr);
                }
            }
        }

        sr->Close();

        if (box != nullptr)
            populateTrackList(box);

       
    }
    catch (Exception^ ex)
    {
        MessageBox::Show("Error loading file: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// Search operations
List<VideoList::Node^>^ VideoList::searchVideos(String^ searchTerm)
{
    List<Node^>^ results = gcnew List<Node^>();

    if (String::IsNullOrWhiteSpace(searchTerm))
        return results;

    String^ lowerSearchTerm = searchTerm->ToLower();
    Node^ temp = head;

    while (temp != nullptr)
    {
        if (!temp->isPlaylistHeader && temp->videoName != nullptr)
        {
            if (temp->videoName->ToLower()->Contains(lowerSearchTerm))
            {
                results->Add(temp);
            }
        }
        temp = temp->next;
    }

    return results;
}

// Sorting operations
void VideoList::sortAlphabetically(ListBox^ box)
{
    if (isEmpty() || size <= 1) return;

    List<Node^>^ allItems = gcnew List<Node^>();
    Dictionary<Node^, List<Node^>^>^ playlistVideosDict = gcnew Dictionary<Node^, List<Node^>^>();

    Node^ temp = head;

    while (temp != nullptr)
    {
        if (temp->isPlaylistHeader)
        {
            allItems->Add(temp);
            playlistVideosDict->Add(temp, gcnew List<Node^>());
        }
        else if (temp->playlistName == nullptr)
        {
            allItems->Add(temp);
        }
        temp = temp->next;
    }

    for each (Node ^ playlistHeader in playlistVideosDict->Keys)
    {
        List<Node^>^ playlistVideos = playlistVideosDict[playlistHeader];
        Node^ scanner = head;

        while (scanner != nullptr)
        {
            if (!scanner->isPlaylistHeader &&
                scanner->playlistName != nullptr &&
                scanner->playlistName == playlistHeader->playlistName)
            {
                playlistVideos->Add(scanner);
            }
            scanner = scanner->next;
        }

        for (int i = 0; i < playlistVideos->Count - 1; i++)
        {
            for (int j = 0; j < playlistVideos->Count - i - 1; j++)
            {
                if (String::Compare(playlistVideos[j]->videoName,
                    playlistVideos[j + 1]->videoName, true) > 0)
                {
                    Node^ tempNode = playlistVideos[j];
                    playlistVideos[j] = playlistVideos[j + 1];
                    playlistVideos[j + 1] = tempNode;
                }
            }
        }
    }

    for (int i = 0; i < allItems->Count - 1; i++)
    {
        for (int j = 0; j < allItems->Count - i - 1; j++)
        {
            String^ name1 = allItems[j]->isPlaylistHeader ?
                allItems[j]->playlistName : allItems[j]->videoName;
            String^ name2 = allItems[j + 1]->isPlaylistHeader ?
                allItems[j + 1]->playlistName : allItems[j + 1]->videoName;

            if (String::Compare(name1, name2, true) > 0)
            {
                Node^ tempNode = allItems[j];
                allItems[j] = allItems[j + 1];
                allItems[j + 1] = tempNode;
            }
        }
    }

    head = nullptr;
    tail = nullptr;
    size = 0;

    for each (Node ^ item in allItems)
    {
        item->next = nullptr;
        appendNode(item);

        if (item->isPlaylistHeader)
        {
            List<Node^>^ videos = playlistVideosDict[item];
            for each (Node ^ video in videos)
            {
                video->next = nullptr;
                appendNode(video);
            }
        }
    }

    if (box != nullptr)
        populateTrackList(box);

    MessageBox::Show("Sorted alphabetically (A-Z).", "Sorted",
        MessageBoxButtons::OK, MessageBoxIcon::Information);
}

void VideoList::sortByTimeAdded(ListBox^ box)
{
    if (isEmpty() || size <= 1) return;

    List<Node^>^ allItems = gcnew List<Node^>();
    Dictionary<Node^, List<Node^>^>^ playlistVideosDict = gcnew Dictionary<Node^, List<Node^>^>();

    Node^ temp = head;

    while (temp != nullptr)
    {
        if (temp->isPlaylistHeader)
        {
            allItems->Add(temp);
            playlistVideosDict->Add(temp, gcnew List<Node^>());
        }
        else if (temp->playlistName == nullptr)
        {
            allItems->Add(temp);
        }
        temp = temp->next;
    }

    for each (Node ^ playlistHeader in playlistVideosDict->Keys)
    {
        List<Node^>^ playlistVideos = playlistVideosDict[playlistHeader];
        Node^ scanner = head;

        while (scanner != nullptr)
        {
            if (!scanner->isPlaylistHeader &&
                scanner->playlistName != nullptr &&
                scanner->playlistName == playlistHeader->playlistName)
            {
                playlistVideos->Add(scanner);
            }
            scanner = scanner->next;
        }

        for (int i = 0; i < playlistVideos->Count - 1; i++)
        {
            for (int j = 0; j < playlistVideos->Count - i - 1; j++)
            {
                if (DateTime::Compare(playlistVideos[j]->addedTime,
                    playlistVideos[j + 1]->addedTime) > 0)
                {
                    Node^ tempNode = playlistVideos[j];
                    playlistVideos[j] = playlistVideos[j + 1];
                    playlistVideos[j + 1] = tempNode;
                }
            }
        }
    }

    for (int i = 0; i < allItems->Count - 1; i++)
    {
        for (int j = 0; j < allItems->Count - i - 1; j++)
        {
            if (DateTime::Compare(allItems[j]->addedTime, allItems[j + 1]->addedTime) > 0)
            {
                Node^ tempNode = allItems[j];
                allItems[j] = allItems[j + 1];
                allItems[j + 1] = tempNode;
            }
        }
    }

    head = nullptr;
    tail = nullptr;
    size = 0;

    for each (Node ^ item in allItems)
    {
        item->next = nullptr;
        appendNode(item);

        if (item->isPlaylistHeader)
        {
            List<Node^>^ videos = playlistVideosDict[item];
            for each (Node ^ video in videos)
            {
                video->next = nullptr;
                appendNode(video);
            }
        }
    }

    if (box != nullptr)
        populateTrackList(box);

    MessageBox::Show("Sorted by time added (oldest first).", "Sorted",
        MessageBoxButtons::OK, MessageBoxIcon::Information);
}

// Utility operations
int VideoList::shuffle()
{
    if (isEmpty()) return -1;

    List<Node^>^ playableVideos = getAllPlayableVideos();
    if (playableVideos->Count == 0) return -1;

    Random^ rand = gcnew Random();
    int index = rand->Next(0, playableVideos->Count);
    current = playableVideos[index];

    return getCurrentNodeIndex();
}

void VideoList::printList()
{
    Node^ temp = head;
    while (temp != nullptr)
    {
        Console::WriteLine(temp->videoName);
        temp = temp->next;
    }
}