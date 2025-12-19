#include "PhotoList.h"

// Node Constructor
PhotoList::Node::Node(String^ path, String^ name, String^ album, bool isHeader)
{
    photoPath = path;
    photoName = name;
    albumName = album;
    isAlbumHeader = isHeader;
    isExpanded = false;
    addedTime = DateTime::Now;
    next = nullptr;
}

// PhotoList Constructor
PhotoList::PhotoList()
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    size = 0;
}

// Private helper methods
void PhotoList::appendNode(Node^ node)
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

bool PhotoList::isDuplicate(String^ path)
{
    Node^ temp = head;
    while (temp != nullptr)
    {
        if (!temp->isAlbumHeader &&
            String::Compare(temp->photoPath, path, true) == 0)
            return true;
        temp = temp->next;
    }
    return false;
}

PhotoList::Node^ PhotoList::getPhotoAt(int index)
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

PhotoList::NodeArray^ PhotoList::getAllPlayablePhotos()
{
    NodeArray^ playablePhotos = gcnew NodeArray();
    Node^ temp = head;

    while (temp != nullptr)
    {
        if (!temp->isAlbumHeader && temp->photoPath != nullptr)
            playablePhotos->add(temp);
        temp = temp->next;
    }

    return playablePhotos;
}

// Basic operations
bool PhotoList::isEmpty()
{
    return head == nullptr;
}

int PhotoList::getSize()
{
    return size;
}

// Add operations
void PhotoList::addAlbum(array<String^>^ photoPaths, String^ albumName, ListBox^ box)
{
    if (photoPaths == nullptr || photoPaths->Length == 0)
    {
        MessageBox::Show("No photos selected for album.", "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    Node^ headerNode = gcnew Node(nullptr, albumName, albumName, true);
    appendNode(headerNode);

    for each (String ^ path in photoPaths)
    {
        if (File::Exists(path))
        {
            String^ name = Path::GetFileName(path);
            Node^ photoNode = gcnew Node(path, name, albumName, false);
            appendNode(photoNode);
        }
    }

    if (box != nullptr)
        populatePhotoList(box);
}

void PhotoList::addPhoto(String^ path, String^ name, ListBox^ box)
{
    if (!File::Exists(path))
    {
        MessageBox::Show("Photo file does not exist!", "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

    if (isDuplicate(path))
    {
        System::Windows::Forms::DialogResult result = MessageBox::Show(
            "This photo already exists in the collection!\n\nDo you want to add it anyway?",
            "Duplicate Photo",
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
        populatePhotoList(box);
}

// Navigation operations
String^ PhotoList::nextPhoto()
{
    if (isEmpty() || current == nullptr) return nullptr;

    if (current->albumName != nullptr && !current->isAlbumHeader)
    {
        Node^ temp = current->next;
        while (temp != nullptr)
        {
            if (!temp->isAlbumHeader && temp->albumName == current->albumName)
            {
                current = temp;
                return current->photoPath;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    Node^ temp = current->next;
    while (temp != nullptr)
    {
        if (!temp->isAlbumHeader && temp->photoPath != nullptr)
        {
            current = temp;
            return current->photoPath;
        }
        temp = temp->next;
    }

    return nullptr;
}

String^ PhotoList::previousPhoto()
{
    if (isEmpty() || current == nullptr) return nullptr;

    Node^ temp = head;
    Node^ prev = nullptr;

    while (temp != nullptr && temp != current)
    {
        if (!temp->isAlbumHeader && temp->photoPath != nullptr)
        {
            if (current->albumName != nullptr)
            {
                if (temp->albumName == current->albumName)
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
        return current->photoPath;
    }

    return nullptr;
}

String^ PhotoList::getRandomPhoto()
{
    if (isEmpty()) return nullptr;

    NodeArray^ playablePhotos = getAllPlayablePhotos();

    if (playablePhotos->count == 0) return nullptr;

    Random^ rnd = gcnew Random();
    int index = rnd->Next(playablePhotos->count);
    current = playablePhotos->get(index);
    return current->photoPath;
}

// Current node operations
String^ PhotoList::getCurrentNodePath()
{
    return (isEmpty() || current == nullptr) ? nullptr : current->photoPath;
}

String^ PhotoList::getCurrentNodeName()
{
    return (isEmpty() || current == nullptr) ? nullptr : current->photoName;
}

void PhotoList::setCurrentNode(Node^ node)
{
    current = node;
}

void PhotoList::setCurrentNode(int index)
{
    Node^ node = getPhotoAt(index);
    if (node != nullptr)
        current = node;
}

PhotoList::Node^ PhotoList::getCurrentNode()
{
    return current;
}

int PhotoList::getCurrentNodeIndex()
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
void PhotoList::populatePhotoList(ListBox^ box)
{
    if (box == nullptr) return;

    box->BeginUpdate();
    box->Items->Clear();

    Node^ temp = head;
    while (temp != nullptr)
    {
        if (temp->isAlbumHeader)
        {
            String^ icon = temp->isExpanded ? "[+] " : "[-] ";
            box->Items->Add(icon + temp->photoName);

            if (temp->isExpanded)
            {
                Node^ photoNode = temp->next;
                while (photoNode != nullptr && !photoNode->isAlbumHeader)
                {
                    if (photoNode->albumName == temp->albumName)
                    {
                        Node^ nextNode = photoNode->next;
                        bool isLast = (nextNode == nullptr || nextNode->isAlbumHeader ||
                            nextNode->albumName != temp->albumName);

                        String^ branch = isLast ? "  |__ " : "  |-- ";
                        box->Items->Add(branch + photoNode->photoName);
                    }
                    photoNode = photoNode->next;
                }
            }
        }
        else if (temp->albumName == nullptr)
        {
            box->Items->Add("* " + temp->photoName);
        }

        temp = temp->next;
    }

    box->EndUpdate();
}

PhotoList::Node^ PhotoList::getNodeAtDisplayIndex(int displayIndex, ListBox^ box)
{
    int currentDisplayIndex = 0;
    Node^ temp = head;

    while (temp != nullptr)
    {
        if (temp->isAlbumHeader)
        {
            if (currentDisplayIndex == displayIndex)
                return temp;
            currentDisplayIndex++;

            if (temp->isExpanded)
            {
                Node^ photoNode = temp->next;
                while (photoNode != nullptr && !photoNode->isAlbumHeader)
                {
                    if (photoNode->albumName == temp->albumName)
                    {
                        if (currentDisplayIndex == displayIndex)
                            return photoNode;
                        currentDisplayIndex++;
                    }
                    photoNode = photoNode->next;
                }
            }
        }
        else if (temp->albumName == nullptr)
        {
            if (currentDisplayIndex == displayIndex)
                return temp;
            currentDisplayIndex++;
        }

        temp = temp->next;
    }

    return nullptr;
}

// Album operations
void PhotoList::toggleAlbumExpansion(int index, ListBox^ box)
{
    Node^ node = getNodeAtDisplayIndex(index, box);
    if (node != nullptr && node->isAlbumHeader)
    {
        node->isExpanded = !node->isExpanded;
        populatePhotoList(box);
    }
}

PhotoList::NodeArray^ PhotoList::getAlbumPhotos(String^ albumName)
{
    NodeArray^ photos = gcnew NodeArray();
    Node^ temp = head;

    while (temp != nullptr)
    {
        if (!temp->isAlbumHeader && temp->albumName == albumName)
        {
            photos->add(temp);
        }
        temp = temp->next;
    }

    return photos;
}

// Remove operations
void PhotoList::removeItem(int displayIndex, ListBox^ box)
{
    Node^ nodeToRemove = getNodeAtDisplayIndex(displayIndex, box);
    if (nodeToRemove == nullptr) return;

    if (nodeToRemove->isAlbumHeader)
    {
        System::Windows::Forms::DialogResult result = MessageBox::Show(
            "Delete album '" + nodeToRemove->photoName + "' and all its photos?",
            "Confirm Delete",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Question
        );

        if (result == System::Windows::Forms::DialogResult::No)
            return;

        String^ albumToRemove = nodeToRemove->albumName;

        Node^ temp = head;
        Node^ prev = nullptr;

        while (temp != nullptr)
        {
            Node^ nextNode = temp->next;

            if ((temp->isAlbumHeader && temp->albumName == albumToRemove) ||
                (!temp->isAlbumHeader && temp->albumName == albumToRemove))
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

    populatePhotoList(box);
}

void PhotoList::clearAll(ListBox^ box)
{
    if (isEmpty()) return;

    System::Windows::Forms::DialogResult result = MessageBox::Show(
        "Are you sure you want to remove all albums and photos?",
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
void PhotoList::saveToFile(String^ filename, ListBox^ box)
{
    try
    {
        StreamWriter^ sw = gcnew StreamWriter(filename);
        sw->WriteLine("# PhotoList Save File v1.0");
        sw->WriteLine("# Generated: " + DateTime::Now.ToString());

        Node^ temp = head;

        while (temp != nullptr)
        {
            if (temp->isAlbumHeader)
            {
                sw->WriteLine("[ALBUM]" + temp->albumName);
            }
            else if (temp->albumName != nullptr)
            {
                sw->WriteLine("[ALBUMPHOTO]" + temp->albumName + "|" + temp->photoPath);
            }
            else
            {
                sw->WriteLine("[PHOTO]" + temp->photoPath);
            }
            temp = temp->next;
        }

        sw->Close();

        MessageBox::Show("Photo list saved successfully!", "Saved",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
    catch (Exception^ ex)
    {
        MessageBox::Show("Error saving file: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void PhotoList::loadFromFile(String^ filename, ListBox^ box)
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

            if (line->StartsWith("[ALBUM]"))
            {
                String^ albumName = line->Substring(7);
                Node^ headerNode = gcnew Node(nullptr, albumName, albumName, true);
                appendNode(headerNode);
            }
            else if (line->StartsWith("[ALBUMPHOTO]"))
            {
                String^ data = line->Substring(12);
                int separatorIndex = data->IndexOf('|');

                if (separatorIndex > 0)
                {
                    String^ albumName = data->Substring(0, separatorIndex);
                    String^ photoPath = data->Substring(separatorIndex + 1);

                    if (File::Exists(photoPath))
                    {
                        String^ photoName = Path::GetFileName(photoPath);
                        Node^ photoNode = gcnew Node(photoPath, photoName, albumName, false);
                        appendNode(photoNode);
                    }
                }
            }
            else if (line->StartsWith("[PHOTO]"))
            {
                String^ photoPath = line->Substring(7);

                if (File::Exists(photoPath))
                {
                    String^ photoName = Path::GetFileName(photoPath);
                    addPhoto(photoPath, photoName, nullptr);
                }
            }
        }

        sr->Close();

        if (box != nullptr)
            populatePhotoList(box);

        MessageBox::Show("Photo list loaded successfully!", "Loaded",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
    catch (Exception^ ex)
    {
        MessageBox::Show("Error loading file: " + ex->Message, "Error",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// Search operations
PhotoList::NodeArray^ PhotoList::searchPhotos(String^ searchTerm)
{
    NodeArray^ results = gcnew NodeArray();

    if (String::IsNullOrWhiteSpace(searchTerm))
        return results;

    String^ lowerSearchTerm = searchTerm->ToLower();
    Node^ temp = head;

    while (temp != nullptr)
    {
        if (!temp->isAlbumHeader && temp->photoName != nullptr)
        {
            if (temp->photoName->ToLower()->Contains(lowerSearchTerm))
            {
                results->add(temp);
            }
        }
        temp = temp->next;
    }

    return results;
}

// Sorting operations - Bubble Sort (NO STL)
void PhotoList::sortAlphabetically(ListBox^ box)
{
    if (isEmpty() || size <= 1) return;

    // Separate items into albums and standalone photos
    Node^ albumsHead = nullptr;
    Node^ albumsTail = nullptr;
    Node^ photosHead = nullptr;
    Node^ photosTail = nullptr;

    Node^ temp = head;
    while (temp != nullptr)
    {
        Node^ nextNode = temp->next;
        temp->next = nullptr;

        if (temp->isAlbumHeader)
        {
            if (albumsHead == nullptr)
            {
                albumsHead = temp;
                albumsTail = temp;
            }
            else
            {
                albumsTail->next = temp;
                albumsTail = temp;
            }
        }
        else if (temp->albumName == nullptr)
        {
            if (photosHead == nullptr)
            {
                photosHead = temp;
                photosTail = temp;
            }
            else
            {
                photosTail->next = temp;
                photosTail = temp;
            }
        }
        temp = nextNode;
    }

    // Bubble sort albums
    if (albumsHead != nullptr)
    {
        bool swapped = true;
        while (swapped)
        {
            swapped = false;
            Node^ current = albumsHead;
            Node^ prev = nullptr;

            while (current != nullptr && current->next != nullptr)
            {
                if (String::Compare(current->albumName, current->next->albumName, true) > 0)
                {
                    Node^ next = current->next;
                    current->next = next->next;
                    next->next = current;

                    if (prev == nullptr)
                        albumsHead = next;
                    else
                        prev->next = next;

                    if (current == albumsTail)
                        albumsTail = next;
                    if (next->next == nullptr)
                        albumsTail = next;

                    swapped = true;
                    prev = next;
                }
                else
                {
                    prev = current;
                    current = current->next;
                }
            }
        }
    }

    // Bubble sort standalone photos
    if (photosHead != nullptr)
    {
        bool swapped = true;
        while (swapped)
        {
            swapped = false;
            Node^ current = photosHead;
            Node^ prev = nullptr;

            while (current != nullptr && current->next != nullptr)
            {
                if (String::Compare(current->photoName, current->next->photoName, true) > 0)
                {
                    Node^ next = current->next;
                    current->next = next->next;
                    next->next = current;

                    if (prev == nullptr)
                        photosHead = next;
                    else
                        prev->next = next;

                    if (current == photosTail)
                        photosTail = next;
                    if (next->next == nullptr)
                        photosTail = next;

                    swapped = true;
                    prev = next;
                }
                else
                {
                    prev = current;
                    current = current->next;
                }
            }
        }
    }

    // Rebuild list
    head = nullptr;
    tail = nullptr;
    size = 0;

    // Add sorted albums with their photos
    Node^ album = albumsHead;
    while (album != nullptr)
    {
        Node^ nextAlbum = album->next;
        album->next = nullptr;
        appendNode(album);

        // Get and sort photos for this album
        Node^ albumPhotosHead = nullptr;
        Node^ albumPhotosTail = nullptr;

        temp = head->next;
        Node^ prev = head;
        while (temp != nullptr)
        {
            Node^ next = temp->next;
            if (!temp->isAlbumHeader && temp->albumName == album->albumName)
            {
                prev->next = next;
                temp->next = nullptr;

                if (albumPhotosHead == nullptr)
                {
                    albumPhotosHead = temp;
                    albumPhotosTail = temp;
                }
                else
                {
                    albumPhotosTail->next = temp;
                    albumPhotosTail = temp;
                }
                temp = next;
            }
            else
            {
                prev = temp;
                temp = next;
            }
        }

        // Bubble sort album photos
        if (albumPhotosHead != nullptr)
        {
            bool swapped = true;
            while (swapped)
            {
                swapped = false;
                Node^ current = albumPhotosHead;
                Node^ prev2 = nullptr;

                while (current != nullptr && current->next != nullptr)
                {
                    if (String::Compare(current->photoName, current->next->photoName, true) > 0)
                    {
                        Node^ next = current->next;
                        current->next = next->next;
                        next->next = current;

                        if (prev2 == nullptr)
                            albumPhotosHead = next;
                        else
                            prev2->next = next;

                        swapped = true;
                        prev2 = next;
                    }
                    else
                    {
                        prev2 = current;
                        current = current->next;
                    }
                }
            }
        }

        // Add sorted photos to main list
        Node^ photo = albumPhotosHead;
        while (photo != nullptr)
        {
            Node^ nextPhoto = photo->next;
            photo->next = nullptr;
            appendNode(photo);
            photo = nextPhoto;
        }

        album = nextAlbum;
    }

    // Add sorted standalone photos
    Node^ photo = photosHead;
    while (photo != nullptr)
    {
        Node^ nextPhoto = photo->next;
        photo->next = nullptr;
        appendNode(photo);
        photo = nextPhoto;
    }

    if (box != nullptr)
        populatePhotoList(box);

    MessageBox::Show("Sorted alphabetically (A-Z).", "Sorted",
        MessageBoxButtons::OK, MessageBoxIcon::Information);
}

void PhotoList::sortByTimeAdded(ListBox^ box)
{
    if (isEmpty() || size <= 1) return;

    // Collect headers and standalone photos, and prepare dictionary for album photos
    System::Collections::Generic::List<Node^>^ allItems = gcnew System::Collections::Generic::List<Node^>();
    System::Collections::Generic::Dictionary<Node^, System::Collections::Generic::List<Node^>^>^ albumPhotosDict =
        gcnew System::Collections::Generic::Dictionary<Node^, System::Collections::Generic::List<Node^>^>();

    Node^ temp = head;
    while (temp != nullptr)
    {
        if (temp->isAlbumHeader)
        {
            allItems->Add(temp);
            albumPhotosDict->Add(temp, gcnew System::Collections::Generic::List<Node^>());
        }
        else if (temp->albumName == nullptr)
        {
            allItems->Add(temp);
        }
        temp = temp->next;
    }

    // Collect photos for each album header
    for each (auto header in albumPhotosDict->Keys)
    {
        Node^ scanner = head;
        while (scanner != nullptr)
        {
            if (!scanner->isAlbumHeader && scanner->albumName != nullptr && scanner->albumName == header->albumName)
            {
                albumPhotosDict[header]->Add(scanner);
            }
            scanner = scanner->next;
        }

        // Bubble sort album photos by addedTime (oldest first)
        auto listRef = albumPhotosDict[header];
        for (int i = 0; i < listRef->Count - 1; i++)
        {
            for (int j = 0; j < listRef->Count - i - 1; j++)
            {
                if (DateTime::Compare(listRef[j]->addedTime, listRef[j + 1]->addedTime) > 0)
                {
                    Node^ t = listRef[j];
                    listRef[j] = listRef[j + 1];
                    listRef[j + 1] = t;
                }
            }
        }
    }

    // Sort allItems (headers + standalone photos) by addedTime (oldest first)
    for (int i = 0; i < allItems->Count - 1; i++)
    {
        for (int j = 0; j < allItems->Count - i - 1; j++)
        {
            if (DateTime::Compare(allItems[j]->addedTime, allItems[j + 1]->addedTime) > 0)
            {
                Node^ t = allItems[j];
                allItems[j] = allItems[j + 1];
                allItems[j + 1] = t;
            }
        }
    }

    // Rebuild linked list
    head = nullptr;
    tail = nullptr;
    size = 0;

    for each (Node^ item in allItems)
    {
        item->next = nullptr;
        appendNode(item);

        if (item->isAlbumHeader)
        {
            auto photos = albumPhotosDict[item];
            for each (Node^ ph in photos)
            {
                ph->next = nullptr;
                appendNode(ph);
            }
        }
    }

    if (box != nullptr)
        populatePhotoList(box);

    MessageBox::Show("Sorted by time added (oldest first).", "Sorted",
        MessageBoxButtons::OK, MessageBoxIcon::Information);
}

// Utility operations
int PhotoList::shuffle()
{
    if (isEmpty()) return -1;

    NodeArray^ playablePhotos = getAllPlayablePhotos();
    if (playablePhotos->count == 0) return -1;

    Random^ rand = gcnew Random();
    int index = rand->Next(0, playablePhotos->count);
    current = playablePhotos->get(index);

    return getCurrentNodeIndex();
}

void PhotoList::printList()
{
    Node^ temp = head;
    while (temp != nullptr)
    {
        Console::WriteLine(temp->photoName);
        temp = temp->next;
    }
}