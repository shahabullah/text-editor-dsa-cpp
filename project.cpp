#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#define MAX 100
using namespace std;

struct Node
{
    string text;
    Node *next;
    Node *prev;
};

struct Action
{
    string type;
    int lineNo;
    string oldText;
    string newText;
};

int lineCount = 0;
Action undoStack[MAX];
Action redoStack[MAX];
int undoTop = -1;
int redoTop = -1;

void pushUndo(Action item)
{
    if (undoTop == MAX - 1)
    {
        cout << "stack overflow";
        return;
    }
    undoTop = undoTop + 1;
    undoStack[undoTop] = item;
}

Action popUndo()
{
    Action item = undoStack[undoTop];
    undoTop = undoTop - 1;
    return item;
}
void pushRedo(Action item)
{
    if (redoTop == MAX - 1)
    {
        cout << "Redo stack overflow";
        return;
    }
    redoTop = redoTop + 1;
    redoStack[redoTop] = item;
}

Action popRedo()
{
    Action item = redoStack[redoTop];
    redoTop = redoTop - 1;
    return item;
}

void insertAtPosition(Node *&head, string text, int position)
{
    Node *newNode = new Node;
    newNode->text = text;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (position == 1)
    {
        newNode->next = head;
        if (head != NULL)
        {
            head->prev = newNode;
        }
        head = newNode;
    }
    else
    {
        Node *temp = head;
        for (int i = 1; i < position - 1; i++)
        {
            temp = temp->next;
        }

        newNode->next = temp->next;
        newNode->prev = temp;

        if (temp->next != NULL)
        {
            temp->next->prev = newNode;
        }
        temp->next = newNode;
    }
    lineCount++;
}

void deleteAtPosition(Node *&head, int position, string &removed)
{
    Node *temp = head;

    for (int i = 1; i < position; i++)
    {
        temp = temp->next;
    }
    removed = temp->text;

    if (temp->prev != NULL)
    {
        temp->prev->next = temp->next;
    }
    else
    {
        head = temp->next;
    }

    if (temp->next != NULL)
    {
        temp->next->prev = temp->prev;
    }
    delete temp;
    lineCount--;
}

void editAtPosition(Node *head, int position, string newText, string &oldText)
{
    Node *temp = head;
    int i = 1;
    while (temp != NULL && i < position)
    {
        temp = temp->next;
        i++;
    }
    oldText = temp->text;
    temp->text = newText;
}

void display(Node *head)
{
    if (head == NULL)
    {
        cout << "empty document";
        return;
    }

    int n = 1;
    for (Node *temp = head; temp != NULL; temp = temp->next)
    {
        cout << n++ << ": " << temp->text << endl;
    }
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.get();
}

void saveToFile(Node *head, string filename)
{
    ofstream file(filename.c_str());
    if (!file)
    {
        cout << "Could not create the file\n";
        return;
    }
    for (Node *temp = head; temp != NULL; temp = temp->next)
    {
        file << temp->text << endl;
    }
    file.close();
    cout << "Saved to " << filename << endl;
}

void loadFromFile(Node *&head, string filename)
{
    ifstream file(filename.c_str());
    if (!file)
    {
        cout << "File not found\n";
        return;
    }

    while (head != NULL)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
    lineCount = 0;

    string line;
    while (getline(file, line))
    {
        insertAtPosition(head, line, lineCount + 1);
    }
    file.close();
    cout << "Loaded from " << filename << endl;
}

int main()
{
    Node *head = NULL;
    int choice, position;
    string text, removed, filename;
    Action item;

    do
    {
        system("cls");
        cout << "\n----------- Text Editor -----------\n";
        cout << "1. Insert line\n";
        cout << "2. Delete line\n";
        cout << "3. Show text\n";
        cout << "4. Undo\n";
        cout << "5. Edit line\n";
        cout << "6. Redo\n";
        cout << "7. Save to file\n";
        cout << "8. Load from file\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            display(head);
            cout << "Enter position (1 to " << lineCount + 1 << "): ";
            cin >> position;
            cin.ignore();
            if (position < 1 || position > lineCount + 1)
            {

                cout << "Invalid position\n";
                break;
            }
            cout << "Enter text: ";
            getline(cin, text);
            insertAtPosition(head, text, position);
            item.type = "INSERT";
            item.lineNo = position;
            item.oldText = "";
            item.newText = text;
            redoTop = -1;
            pushUndo(item);
            break;

        case 2:
            display(head);
            cout << "Enter line number to delete: ";
            cin >> position;
            cin.ignore();
            if (position < 1 || position > lineCount)
            {
                cout << "Invalid line number\n";
                break;
            }
            deleteAtPosition(head, position, removed);
            item.type = "DELETE";
            item.lineNo = position;
            item.oldText = removed;
            item.newText = "";
            redoTop = -1;
            pushUndo(item);
            break;

        case 3:
            display(head);
            break;

        case 4:
            if (undoTop == -1)
            {
                cout << "Nothing to undo\n";
                break;
            }
            item = popUndo();
            if (item.type == "INSERT")
            {
                deleteAtPosition(head, item.lineNo, removed);
            }
            else if (item.type == "DELETE")
            {
                insertAtPosition(head, item.oldText, item.lineNo);
            }
            else if (item.type == "EDIT")
            {
                editAtPosition(head, item.lineNo, item.oldText, removed);
            }
            pushRedo(item);
            cout << "Undo done\n";
            break;

        case 5:
            if (lineCount == 0)
            {
                cout << "Nothing to edit\n";
                break;
            }
            display(head);
            cout << "Enter Line num to edit: (1 to " << lineCount << "): ";
            cin >> position;
            cin.ignore();
            if (position < 1 || position > lineCount)
            {
                cout << "Invalid line number\n";
                break;
            }
            cout << "Ente new Text: ";
            getline(cin, text);
            editAtPosition(head, position, text, removed);
            item.type = "EDIT";
            item.lineNo = position;
            item.oldText = removed;
            item.newText = text;
            redoTop = -1;
            pushUndo(item);
            cout << "\nLine edited. Document now:\n";
            display(head);
            break;

        case 6:
            if (redoTop == -1)
            {
                cout << "Nothing to redo\n";
                break;
            }
            item = popRedo();
            if (item.type == "INSERT")
            {
                insertAtPosition(head, item.newText, item.lineNo);
            }
            else if (item.type == "DELETE")
            {
                deleteAtPosition(head, item.lineNo, removed);
            }
            else if (item.type == "EDIT")
            {
                editAtPosition(head, item.lineNo, item.newText, removed);
            }
            pushUndo(item);
            cout << "Redo Done\n";
            break;

        case 7:
            if (lineCount == 0)
            {
                cout << "Nothing to save\n";
                break;
            }

            cout << "Enter file name (without.text): ";
            getline(cin, filename);
            saveToFile(head, filename + ".txt");

            break;

        case 8:
            cout << "Enter file name to load (without .txt): ";
            getline(cin, filename);
            loadFromFile(head, filename + ".txt");
            undoTop = -1;
            redoTop = -1;
            break;

        case 0:
            cout << "Exiting\n";
            break;
            
        default:
            cout << "Invalide Input\n";
        }
        if (choice != 0)
        {
            pauseScreen();
        }

    } while (choice != 0);

    return 0;
}