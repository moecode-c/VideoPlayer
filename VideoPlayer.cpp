#include <iostream>
#include <string>
using namespace std;

// Video class to represent a video
class Video {
public:
    string title;
    string url;
    int duration; // in seconds
    
    Video(string t, string u, int d) : title(t), url(u), duration(d) {}
    
    void display() {
        cout << "Title: " << title << endl;
        cout << "URL: " << url << endl;
        cout << "Duration: " << duration << " seconds" << endl;
    }
};

// Node class for singly linked list
class Node {
public:
    Video* video;
    Node* next;
    
    Node(Video* v) : video(v), next(nullptr) {}
};

// VideoPlayer class using singly linked list
class VideoPlayer {
private:
    Node* head;
    Node* current;
    int size;
    
public:
    VideoPlayer() : head(nullptr), current(nullptr), size(0) {}
    
    // Destructor to free memory
    ~VideoPlayer() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* next = temp->next;
            delete temp->video;
            delete temp;
            temp = next;
        }
    }
    
    // Add video to the end of playlist
    void addVideo(string title, string url, int duration) {
        Video* video = new Video(title, url, duration);
        Node* newNode = new Node(video);
        
        if (head == nullptr) {
            head = newNode;
            current = head;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
        cout << "Added: " << title << " to playlist" << endl;
    }
    
    // Remove video by title
    void removeVideo(string title) {
        if (head == nullptr) {
            cout << "Playlist is empty!" << endl;
            return;
        }
        
        // If head needs to be removed
        if (head->video->title == title) {
            Node* temp = head;
            head = head->next;
            if (current == temp) {
                current = head;
            }
            delete temp->video;
            delete temp;
            size--;
            cout << "Removed: " << title << " from playlist" << endl;
            return;
        }
        
        // Search for the node to remove
        Node* temp = head;
        while (temp->next != nullptr && temp->next->video->title != title) {
            temp = temp->next;
        }
        
        if (temp->next == nullptr) {
            cout << "Video not found: " << title << endl;
            return;
        }
        
        Node* nodeToDelete = temp->next;
        temp->next = nodeToDelete->next;
        if (current == nodeToDelete) {
            current = temp->next ? temp->next : head;
        }
        delete nodeToDelete->video;
        delete nodeToDelete;
        size--;
        cout << "Removed: " << title << " from playlist" << endl;
    }
    
    // Play next video
    void playNext() {
        if (current == nullptr) {
            cout << "Playlist is empty!" << endl;
            return;
        }
        
        if (current->next != nullptr) {
            current = current->next;
            cout << "\n=== Playing Next Video ===" << endl;
            current->video->display();
        } else {
            cout << "End of playlist reached!" << endl;
        }
    }
    
    // Play current video
    void playCurrent() {
        if (current == nullptr) {
            cout << "Playlist is empty!" << endl;
            return;
        }
        
        cout << "\n=== Now Playing ===" << endl;
        current->video->display();
    }
    
    // Display entire playlist
    void displayPlaylist() {
        if (head == nullptr) {
            cout << "\nPlaylist is empty!" << endl;
            return;
        }
        
        cout << "\n=== Playlist ===" << endl;
        Node* temp = head;
        int index = 1;
        while (temp != nullptr) {
            cout << "\n" << index << ". ";
            if (temp == current) {
                cout << "[CURRENT] ";
            }
            temp->video->display();
            temp = temp->next;
            index++;
        }
        cout << "\nTotal videos: " << size << endl;
    }
    
    // Reset to first video
    void reset() {
        current = head;
        if (current != nullptr) {
            cout << "Reset to first video" << endl;
        }
    }
    
    // Get playlist size
    int getSize() {
        return size;
    }
};

// Main function to demonstrate VideoPlayer
int main() {
    VideoPlayer player;
    
    cout << "=== Video Player using Singly Linked List ===" << endl;
    cout << "\nAdding videos to playlist..." << endl;
    
    // Add some videos
    player.addVideo("Introduction to C++", "https://example.com/cpp-intro", 600);
    player.addVideo("Data Structures", "https://example.com/data-structures", 1200);
    player.addVideo("Linked Lists Tutorial", "https://example.com/linked-lists", 900);
    player.addVideo("Advanced C++ Concepts", "https://example.com/advanced-cpp", 1500);
    
    // Display playlist
    player.displayPlaylist();
    
    // Play current video
    player.playCurrent();
    
    // Play next video
    player.playNext();
    
    // Play next video again
    player.playNext();
    
    // Display playlist
    player.displayPlaylist();
    
    // Remove a video
    cout << "\nRemoving 'Data Structures' from playlist..." << endl;
    player.removeVideo("Data Structures");
    
    // Display playlist after removal
    player.displayPlaylist();
    
    // Reset to first video
    cout << "\nResetting to first video..." << endl;
    player.reset();
    player.playCurrent();
    
    return 0;
}
