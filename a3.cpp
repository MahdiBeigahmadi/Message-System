// a3.cpp

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <mahdi beigahmadi>
// St.# : <301570853>
// Email: <mba188@sfu.ca>
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not #include any other files!
//
#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Node
{
public:
    Announcement data;
    Node *previous;
    Node *next;
    Node(Announcement value) : data(value),
                               previous(nullptr), next(nullptr)
    {
    }
};

class Queue : public Queue_base<Announcement>
{
public:
    Node *frontPointer;
    Node *rear;
    unsigned int counter = 0;
    Queue() : frontPointer(nullptr), rear(nullptr) {}
    bool isEmpty() const
    {
        if (frontPointer == nullptr)
        {
            return true;
        }
        else
            return false;
    }
    using T = Announcement;
    void enqueue(const T &item) override
    {
        Node *newNode = new Node(item);
        if (rear == nullptr)
        {
            frontPointer = rear = newNode;
            counter++;
            return;
        }
        rear->next = newNode;
        newNode->previous = rear;
        rear = newNode;
        counter++;
    }
    void dequeue() override
    {
        if (isEmpty())
        {
            cout << "The queue is empty and not able to access the element\a\n";
            throw runtime_error("Queue is empty!");
        }
        Node *temp = frontPointer;
        if (frontPointer == rear)
        {
            frontPointer = rear = nullptr;
        }
        else
        {
            frontPointer = frontPointer->next;
            frontPointer->previous = nullptr;
        }
        counter--;
        delete temp;
    }

    const T &front() const override
    {
        Node *temp = frontPointer;
        if (isEmpty())
        {
            cout << "The queue is empty and not able to access the peek element\a\n";
            throw runtime_error("Queue is empty!");
        }
        return temp->data;
    }
    ~Queue()
    {
        while (!isEmpty())
        {
            dequeue();
        }
    }
    int size() const override
    {
        return counter;
    }
    void removeAllFromSender(const string &sender)
    {
        Node *current = frontPointer;
        while (current)
        {
            if (current->data.get_sender_name() == sender)
            {
                if (current == frontPointer && current == rear)
                {
                    frontPointer = rear = nullptr;
                    delete current;
                    counter--;
                    return;
                }
                else if (current == frontPointer)
                {
                    frontPointer = frontPointer->next;
                    frontPointer->previous = nullptr;
                    delete current;
                    counter--;
                    current = frontPointer;
                }
                else if (current == rear)
                {
                    rear = rear->previous;
                    rear->next = nullptr;
                    delete current;
                    counter--;
                    return;
                }
                else
                {
                    current->previous->next = current->next;
                    current->next->previous = current->previous;
                    Node *temp = current;
                    current = current->next;
                    delete temp;
                    counter--;
                }
            }
            else
            {
                current = current->next;
            }
        }
    }
};

class JingleNet : public Queue
{
private:
    Queue santa;
    Queue reindeer;
    Queue elf2;
    Queue elf1;
    Queue snowman;

public:
    JingleNet() {}

    bool SEND(string senderName, string rankOfUser, string txtMess)
    {

        string lowerRank = rankOfUser;
        for (char &c : lowerRank)
        {
            if ('A' <= c && c <= 'Z')
            {
                c = c - 'A' + 'a';
            }
        }

        Rank rank;

        if (lowerRank == "santa")
        {
            rank = Rank::SANTA;
        }
        else if (lowerRank == "reindeer")
        {
            rank = Rank::REINDEER;
        }
        else if (lowerRank == "elf2")
        {
            rank = Rank::ELF2;
        }
        else if (lowerRank == "elf1")
        {
            rank = Rank::ELF1;
        }
        else if (lowerRank == "snowman")
        {
            rank = Rank::SNOWMAN;
        }
        else
        {
            cerr << "Invalid rank in string: " << rankOfUser << endl;
            return false;
        }

        Announcement sendObject(senderName, rank, txtMess);

        switch (rank)
        {
        case Rank::SANTA:
            santa.enqueue(sendObject);
            break;
        case Rank::REINDEER:
            reindeer.enqueue(sendObject);
            break;
        case Rank::ELF2:
            elf2.enqueue(sendObject);
            break;
        case Rank::ELF1:
            elf1.enqueue(sendObject);
            break;
        case Rank::SNOWMAN:
            snowman.enqueue(sendObject);
            break;
        default:
            cerr << "Failed to send" << endl;
            return false;
        }

        return true;
    }

    void REMOVE_ALL(const string &removeUser)
    {
        santa.removeAllFromSender(removeUser);
        reindeer.removeAllFromSender(removeUser);
        elf2.removeAllFromSender(removeUser);
        elf1.removeAllFromSender(removeUser);
        snowman.removeAllFromSender(removeUser);
    }
    void PROMOTE_ANNOUNCEMENTS(const string &promoteUser)
    {
        Node *temp = snowman.frontPointer;
        while (temp)
        {
            if (temp->data.get_sender_name() == promoteUser)
            {
                elf1.enqueue(temp->data);
                temp = temp->next;
                snowman.dequeue();
            }
            else
            {
                temp = temp->next;
            }
        }

        Node *temp2 = elf1.frontPointer;
        while (temp2)
        {
            if (temp2->data.get_sender_name() == promoteUser)
            {
                elf2.enqueue(temp2->data);
                temp2 = temp2->next;
                elf1.dequeue();
            }
            else
            {
                temp2 = temp2->next;
            }
        }
        Node *temp3 = elf2.frontPointer;
        while (temp3)
        {
            if (temp3->data.get_sender_name() == promoteUser)
            {
                reindeer.enqueue(temp3->data);
                temp3 = temp3->next;
                elf2.dequeue();
            }
            else
            {
                temp3 = temp3->next;
            }
        }
        Node *temp4 = reindeer.frontPointer;
        while (temp4)
        {
            if (temp4->data.get_sender_name() == promoteUser)
            {
                santa.enqueue(temp4->data);
                temp4 = temp4->next;
                reindeer.dequeue();
            }
            else
            {
                temp4 = temp4->next;
            }
        }
    }

    void ANNOUNCE(unsigned int const announMess)
    {
        unsigned int announced = 0;

        auto announceFromQueue = [&announced, announMess](Queue &queueObject)
        {
            while (!queueObject.isEmpty() && announced < announMess)
            {
                jnet.announce(queueObject.front());
                queueObject.dequeue();
                announced++;
            }
        };
        announceFromQueue(santa);
        announceFromQueue(reindeer);
        announceFromQueue(elf2);
        announceFromQueue(elf1);
        announceFromQueue(snowman);
    }

    void printAllQueuesWithIndices() const
    {
        int index = 1;
        Node *current;

        current = santa.frontPointer;
        while (current)
        {
            cout << index << ": {" << current->data.get_sender_name()
                 << ", " << current->data.get_rank()
                 << ", \"" << current->data.get_text() << "\"}" << endl;
            current = current->next;
            index++;
        }

        current = reindeer.frontPointer;
        while (current)
        {
            cout << index << ": {" << current->data.get_sender_name()
                 << ", " << current->data.get_rank()
                 << ", \"" << current->data.get_text() << "\"}" << endl;
            current = current->next;
            index++;
        }

        current = elf2.frontPointer;
        while (current)
        {
            cout << index << ": {" << current->data.get_sender_name()
                 << ", " << current->data.get_rank()
                 << ", \"" << current->data.get_text() << "\"}" << endl;
            current = current->next;
            index++;
        }

        current = elf1.frontPointer;
        while (current)
        {
            cout << index << ": {" << current->data.get_sender_name()
                 << ", " << current->data.get_rank()
                 << ", \"" << current->data.get_text() << "\"}" << endl;
            current = current->next;
            index++;
        }

        current = snowman.frontPointer;
        while (current)
        {
            cout << index << ": {" << current->data.get_sender_name()
                 << ", " << current->data.get_rank()
                 << ", \"" << current->data.get_text() << "\"}" << endl;
            current = current->next;
            index++;
        }
    }
};

bool processSendCommand(JingleNet &jingleNet, const string &args)
{
    size_t space1Pos = args.find(' ');
    size_t space2Pos = args.find(' ', space1Pos + 1);

    if (space1Pos == string::npos || space2Pos == string::npos ||
        space1Pos >= args.length() || space2Pos >= args.length())
    {
        cout << "Invalid SEND command: " << args << endl;
        return false;
    }

    string sender = args.substr(0, space1Pos);
    string rank = args.substr(space1Pos + 1, space2Pos - space1Pos - 1);
    string message = args.substr(space2Pos + 1);

    return jingleNet.SEND(sender, rank, message);
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <input_file.txt>" << endl;
        return 1;
    }

    JingleNet jingleNet;
    string line;
    ifstream inputFile(argv[1]);

    if (!inputFile.is_open())
    {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    cout << "Processing commands from file: " << argv[1] << endl;

    while (getline(inputFile, line))
    {
        size_t spacePos = line.find(' ');
        if (spacePos == string::npos)
        {
            std::cerr << "Invalid command: " << line << endl;
            continue;
        }

        string command = line.substr(0, spacePos);
        string restOfLine = line.substr(spacePos + 1);

        if (command == "SEND")
        {
            if (!processSendCommand(jingleNet, restOfLine))
            {
                continue;
            }
        }
        else if (command == "REMOVE_ALL")
        {
            jingleNet.REMOVE_ALL(restOfLine);
        }
        else if (command == "PROMOTE_ANNOUNCEMENTS")
        {
            jingleNet.PROMOTE_ANNOUNCEMENTS(restOfLine);
        }
        else if (command == "ANNOUNCE")
        {
            try
            {
                int n = stoi(restOfLine);
                jingleNet.ANNOUNCE(n);
            }
            catch (const invalid_argument &e)
            {
                cout << "Error: Invalid number provided for ANNOUNCE command." << endl;
            }
        }
        else
        {
            cout << "Invalid command: " << command << endl;
        }
    }
    inputFile.close();
    return 0;
}
