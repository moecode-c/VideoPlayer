#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

class Video
{
public:
    std::string path;
    Video(const std::string &p) : path(p) {}
};

class Node
{
public:
    Video *video;
    Node *next;
    Node(Video *v) : video(v), next(nullptr) {}
};
typedef Node *nodeptr;

class VideoPlayer
{
private:
    nodeptr first;
    nodeptr current;
    bool isPlaying;
    bool isPaused;

public:
    VideoPlayer() : first(nullptr), current(nullptr), isPlaying(false), isPaused(false) {}

    ~VideoPlayer() { clear(); }

    void addVideo(const std::string &path)
    {
        Video *v = new Video(path);
        nodeptr n = new Node(v);
        if (first == nullptr)
        {
            first = n;
        }
        else
        {
            nodeptr temp = first;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = n;
        }
    }

    void deleteVideo(const std::string &path)
    {
        nodeptr temp = first;
        nodeptr prev = nullptr;
        while (temp != nullptr)
        {
            if (temp->video->path == path)
            {
                if (prev != nullptr)
                    prev->next = temp->next;
                else
                    first = temp->next;
                if (current == temp)
                    current = temp->next;
                delete temp->video;
                delete temp;
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }

    void displayPlaylist()
    {
        nodeptr temp = first;
        int idx = 1;
        while (temp != nullptr)
        {
            std::cout << idx++ << ". " << temp->video->path << std::endl;
            temp = temp->next;
        }
    }

    void play()
    {
        if (current == nullptr)
            current = first;
        if (current != nullptr)
        {
            isPlaying = true;
            isPaused = false;
            std::cout << "Playing: " << current->video->path << std::endl;
        }
    }

    void pause()
    {
        if (isPlaying && isPaused == false)
        {
            isPaused = true;
            std::cout << "Paused: " << current->video->path << std::endl;
        }
    }

    void stop()
    {
        if (isPlaying == true)
        {
            isPlaying = false;
            isPaused = false;
            std::cout << "Stopped." << std::endl;
        }
    }

    void next()
    {
        if (current != nullptr && current->next != nullptr)
        {
            current = current->next;
            play();
        }
    }

    void previous()
    {
        if (current != nullptr && current != first)
        {
            nodeptr temp = first;
            while (temp->next != current)
                temp = temp->next;
            current = temp;
            play();
        }
    }

    void shuffle()
    {
        std::vector<std::string> paths;
        nodeptr temp = first;
        while (temp != nullptr)
        {
            paths.push_back(temp->video->path);
            temp = temp->next;
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(paths.begin(), paths.end(), g);
        clear();
        for (const auto &p : paths)
            addVideo(p);
        current = first;
    }

    nodeptr search(const std::string &path)
    {
        nodeptr temp = first;
        while (temp != nullptr)
        {
            if (temp->video->path == path)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void savePlaylist(const std::string &filename)
    {
        std::ofstream file(filename);
        nodeptr temp = first;
        while (temp != nullptr)
        {
            file << temp->video->path << std::endl;
            temp = temp->next;
        }
        file.close();
    }

    void loadPlaylist(const std::string &filename)
    {
        clear();
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line))
        {
            addVideo(line);
        }
        file.close();
        current = first;
    }

    void clear()
    {
        nodeptr temp = first;
        while (temp != nullptr)
        {
            nodeptr next = temp->next;
            delete temp->video;
            delete temp;
            temp = next;
        }
        first = nullptr;
        current = nullptr;
        isPlaying = false;
        isPaused = false;
    }
};

// Example usage
int main()
{
    VideoPlayer vp;
    vp.addVideo("video1.mp4");
    vp.addVideo("video2.mp4");
    vp.displayPlaylist();
    vp.play();
    vp.next();
    vp.pause();
    vp.stop();
    vp.savePlaylist("playlist.txt");
    vp.loadPlaylist("playlist.txt");
    vp.shuffle();
    vp.displayPlaylist();
    return 0;
}
