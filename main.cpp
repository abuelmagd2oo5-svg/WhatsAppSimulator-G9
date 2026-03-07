#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;

// ========================
//       USER CLASS
// ========================
class User {
private:
    string username;
    string password;
    string phoneNumber;
    string status;
    string lastSeen;

    bool validPassword(string pass){
        return pass.length() >= 6;
    }

public:
    User() {
        // TODO: Implement default constructor
        username = "";
        password = "";
        phoneNumber = "";
        status = "Offline";
        updateLastSeen();
    }

    User(string uname, string pwd, string phone) {
        // TODO: Implement parameterized constructor
        username = uname;
        while(!validPassword(pwd)){
            cout << "Password must be at least 6 characters" <<endl;
            cout << "Enter password again: ";
            cin >> pwd;
        }
        password = pwd;
        phoneNumber = phone;
        status = "Online";
        updateLastSeen();
    }

    string getUsername() const {
        // TODO: Implement getter
        return username;
    }

    string getPhoneNumber() const {
        // TODO: Implement getter
        return phoneNumber;
    }

    string getStatus() const {
        // TODO: Implement getter
        return status;
    }

    string getLastSeen() const {
        // TODO: Implement getter
        return lastSeen;
    }

    void setStatus(string newStatus) {
        // TODO: Implement setter
        status = newStatus;
    }

    void setPhoneNumber(string phone) {
        // TODO: Implement setter
        phoneNumber = phone;
    }

    void updateLastSeen() {
        // TODO: Implement last seen update
        time_t now = time(0);
        char* date = ctime(&now);
        lastSeen = string(date);
        lastSeen.pop_back();
    }

    bool checkPassword(string pwd) const {
        // TODO: Implement password check
        return (pwd == password);
    }

    void changePassword(string newPwd) {
        // TODO: Implement password change
        while(!validPassword(newPwd)){
            cout << "Password must be at least 6 characters" << endl;
            cout << "Enter new password again: ";
            cin >> newPwd;
        }
        password = newPwd;
    }
};

// ========================
//      MESSAGE CLASS
// ========================
class Message {
private:
    string sender;
    string content;
    string timestamp;
    string status;
    Message* replyTo;

public:
    Message() {
        // TODO: Implement default constructor
        sender="";
        content="";
        status="Sent";
        replyTo =nullptr;
        updateTimestamp();

    }


    Message(string sndr, string cntnt) {
        // TODO: Implement parameterized constructor
        sender= sndr;
        content= cntnt;
        status="Sent";
        replyTo=nullptr;
        updateTimestamp();
    }


    string getContent() const {
        return content;
    }


    string getSender() const {
        return sender;
    }

    string getTimestamp() const {
        return timestamp;
    }

    string getStatus() const {
        return status;
    }

    Message* getReplyTo() const {
        return replyTo;
    }

    void setStatus(string newStatus) {
        status= newStatus;
    }


    void setReplyTo(Message* msg) {
        replyTo= msg;
    }

    void updateTimestamp() {
        time_t now =time(0);
        char* date =ctime(&now);
        timestamp = string(date);
        timestamp.pop_back();
    }

    void display() const {
        cout << sender << " [ " << timestamp << " ] :  " << content;

        if(replyTo !=nullptr){
            cout << "\n (Reply to: "<< replyTo->getContent()<< " )";
        }
        cout << "\nStatus: "<< status << endl;
    }

    void addEmoji(string emojiCode) {
        content += " "+emojiCode;
    }
};



// ========================
//       CHAT CLASS (BASE)
// ========================
class Chat {
protected:
    vector<string> participants;
    vector<Message> messages;
    string chatName;

public:
    Chat() {
       participants = {} ;
       messages = {} ;
       chatName = "" ;
    }

    Chat(vector<string> users, string name) {
        participants = users ;
        chatName = name ;
    }

    void addMessage(const Message& msg) {
        messages.push_back(msg) ;
    }

    bool deleteMessage(int index, const string& username) {
        if (index < 0 || index >= messages.size())
            return false ;

        if (messages[index].getSender() == username)
        {
            messages.erase(messages.begin() + index) ;
            return true ;
        }

        return false;
    }

    virtual void displayChat() const {
        cout<<"Chat Name: " << chatName << endl ;
        cout<<"Participants: " ;
        for (const auto& user : participants)
        {
            cout<< user << " " ;
        }
        cout << endl << "--------------------" << endl ;
        for (const auto& msg : messages)
        {
            msg.display() ;
        }
    }

    vector<Message> searchMessages(string keyword) const {
        vector<Message> result ;

        for(const Message& msg : messages)
        {
            if (msg.getContent().find(keyword) != string::npos)
            {
                result.push_back(msg);
            }

        }
        return result;
    }

    void exportToFile(const string& filename) const {
    ofstream file(filename);

    if(!file){
        cout << "Error opening file!" << endl;
        return;
    }

    file << "Chat Name: " << chatName << endl;
    file << "Participants: ";

    for(const string& user : participants){
        file << user << " ";
    }

    file << endl << endl;

    for(const Message& msg : messages){
        file << msg.getSender() << ": " << msg.getContent() << endl;
    }

    file.close();
    }
};

// ========================
//     PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat {
private:
    string user1;
    string user2;

public:
    PrivateChat(string u1, string u2) {
        user1 = u1 ;
        user2 = u2 ;

        participants.push_back(user1) ;
        participants.push_back(user2) ;

        chatName = "chat between " + user1 + " and " + user2 ;
    }

    void displayChat() const override {
        cout<< "=== private Chat ===" << endl ;
        cout<< chatName << endl ;
        cout<< "--------------------" << endl ;
        for (const auto& msg : messages)
        {
            msg.display() ;
        }
        cout<< "--------------------" << endl ;
    }

    void showTypingIndicator(const string& username) const {
        cout<< username << " is typing..." << endl ;
    }
};

// ========================
//      GROUP CHAT CLASS
// ========================
class GroupChat : public Chat {
private:
    vector<string> admins;
    string description;

public:
    GroupChat(vector<string> users, string name, string creator) {
      
        // TODO: Implement constructor 
        participants = users;
        chatName = name;
        participants.push_back(creator);
        admins.push_back(creator);
    }

    void addAdmin(string newAdmin)
    {
        // TODO: Implement add admin
        
        for (int i = 0; i < participants.size(); i++)
        {
            if (participants[i] == newAdmin && !isAdmin(newAdmin))
            {
                admins.push_back(newAdmin);
            break; 
            }
           
        }
      
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        // TODO: Implement remove participant
         for (int i = 0; i < admins.size (); i++)
        { 
             if (admins[i] == admin)
             {
                 for (int j = 0; j < participants.size(); j++)
                 {
                     if (participants[j] == userToRemove)
                     {
                         participants.erase(participants.begin() + j);
                         return true;
                     }

                 }
              return false;
             }
         
        }
          return false;
    }

    bool isAdmin(string username) const
    {
        // TODO: Implement admin check
        for (int k = 0; k < admins.size(); k++)
        {
            if (admins[k] == username)
            {
                return true;

            }
      
        }
        return false;
    }

    bool isParticipant(string username) const
    {
        // TODO: Implement participant check
        for (int k = 0; k < participants.size(); k++)
        {
            if (participants[k] == username)
            {
                return true;

            }

        }
        return false;
    }

    void setDescription(string desc) {
        // TODO: Implement set description
        description = desc;
    }

    void displayChat() const override {
        // TODO: Implement group chat display
        for (int i = 0; i < messages.size(); i++)
        {
            cout << messages[i] <<  endl ;
        }
            
    }

    void sendJoinRequest(const string& username)
    {
        // TODO: Implement join request
 
        cout << username << " requested to join the group." << endl;
    }
};

// ========================
//    WHATSAPP APP CLASS
// ========================
class WhatsApp {
private:
    vector<User> users;
    vector<Chat*> chats;
    int currentUserIndex;

    int findUserIndex(string username) const {
        // TODO: Implement user search
        return -1;
    }

    bool isLoggedIn() const {
        // TODO: Implement login check
        return false;
    }

    string getCurrentUsername() const {
        // TODO: Implement get current user
        return "";
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    void signUp() {
        // TODO: Implement user registration
    }

    void login() {
        // TODO: Implement user login
    }

    void startPrivateChat() {
        // TODO: Implement private chat creation
    }

    void createGroup() {
        // TODO: Implement group creation
    }

    void viewChats() const {
        // TODO: Implement chat viewing
    }

    void logout() {
        // TODO: Implement logout
    }

    void run() {
        while (true) {
            if (!isLoggedIn()) {
                cout << "\n1. Login\n2. Sign Up\n3. Exit\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) login();
                else if (choice == 2) signUp();
                else if (choice == 3) break;
            }
            else {
                cout << "\n1. Start Private Chat\n2. Create Group\n3. View Chats\n4. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) startPrivateChat();
                else if (choice == 2) createGroup();
                else if (choice == 3) viewChats();
                else if (choice == 4) logout();
            }
        }
    }
};

// ========================
//          MAIN
// ========================
int main() {
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}
