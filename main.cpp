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
        if (index < 0 || index >= (int) messages.size())
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

    vector<Message>& getMessages() {
    return messages;
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

    string getUser1() const { return user1; }
    string getUser2() const { return user2; }

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

        if(!isParticipant(creator))
        {
            participants.push_back(creator);
        }
        admins.push_back(creator);
    }

    void addAdmin(string newAdmin)
    {
        // TODO: Implement add admin

        if(isParticipant(newAdmin) && !isAdmin(newAdmin))
        {
            admins.push_back(newAdmin);
        }

    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        // TODO: Implement remove participant
        if(!isAdmin(admin))
            return false;

        for(int i = 0; i < participants.size(); i++)
        {
            if(participants[i] == userToRemove)
            {
                participants.erase(participants.begin() + i);

                for(int j = 0; j < admins.size(); j++)
                {
                    if(admins[j] == userToRemove)
                    {
                        admins.erase(admins.begin() + j);
                        break;
                    }
                }

                return true;
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

        cout << "=== Group Chat ===" << endl;
        cout << "Group Name: " << chatName << endl;
        cout << "Description: " << description << endl;

        cout << "\nAdmins: ";
        for(const string& admin : admins){
            cout << admin << " ";
        }

        cout << "\nParticipants: ";
        for(const string& user : participants){
            if(!isAdmin(user))
            cout << user << " ";
        }

        cout << endl << "--------------------" << endl;

        for (const Message& msg : messages)
        {
            msg.display();
        }
        cout << "--------------------\n";
    }

    void sendJoinRequest(const string& username)
    {
        // TODO: Implement join request
        if(isParticipant(username)){
            cout << username << " is already in the group.\n";
            return;
        }

        cout << username << " requested to join the group." << endl;
    }


        bool addParticipant(const string& admin, const string& newUser) {
            if (!isAdmin(admin)) {
                cout << "Only admins can add users.\n";
                return false;
            }

            if (isParticipant(newUser)) {
                cout << newUser << " is already in the group.\n";
                return false;
            }

            participants.push_back(newUser);
            cout << newUser << " has been added to the group.\n";
            return true;
        }


        bool makeAdmin(const string& admin, const string& userToPromote) {
            if (!isAdmin(admin)) {
                cout << "Only admins can promote users.\n";
                return false;
            }

            if (!isParticipant(userToPromote)) {
                cout << "User not found in the group.\n";
                return false;
            }

            if (isAdmin(userToPromote)) {
                cout << userToPromote << " is already an admin.\n";
                return false;
            }

            admins.push_back(userToPromote);
            cout << userToPromote << " has been promoted to admin.\n";
            return true;
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
        for(int i = 0; i < users.size(); i++){
            if(users[i].getUsername() == username)
                return i;
        }
        return -1;
    }

    bool isLoggedIn() const {
        // TODO: Implement login check
        return currentUserIndex != -1;
    }

    string getCurrentUsername() const {
        // TODO: Implement get current user
        if(currentUserIndex != -1)
        return users[currentUserIndex].getUsername();

        return "";
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    void signUp() {
        // TODO: Implement user registration
        string username, password, phone;

        cout << "Enter username: ";
        cin >> username;

        if(findUserIndex(username) != -1){
            cout << "Username already exists.\n";
            return;
        }

        cout << "Enter password: ";
        cin >> password;

        cout << "Enter phone number: ";
        cin >> phone;

        User newUser(username, password, phone);
        users.push_back(newUser);

        cout << "Account created successfully.\n";
    }


    void login() {
        // TODO: Implement user login
        string username, password;

        cout << "Enter username: ";
        cin >> username;

        cout << "Enter password: ";
        cin >> password;

        int index = findUserIndex(username);

        if(index == -1){
            cout << "User not found.\n";
            return;
        }

        if(users[index].checkPassword(password)){
            currentUserIndex = index;
            users[index].setStatus("Online");
            cout << "Login successful.\n";
        }
        else{
            cout << "Incorrect password.\n";
        }
    }

    void startPrivateChat() {
        // TODO: Implement private chat creation
        string otherUser;

        cout << "Enter username to chat with: ";
        cin >> otherUser;

        if(otherUser == getCurrentUsername()){
            cout << "You cannot chat with yourself.\n";
            return;
        }

        if(findUserIndex(otherUser) == -1){
            cout << "User not found.\n";
            return;
        }

        string currentUser = getCurrentUsername();

        string chatName1 = "chat between " + currentUser + " and " + otherUser;
        string chatName2 = "chat between " + otherUser + " and " + currentUser;


        for(Chat* chat : chats){
            PrivateChat* pc = dynamic_cast<PrivateChat*>(chat);
            if(pc != nullptr){
                if((pc->getUser1() == currentUser && pc->getUser2() == otherUser) ||
                    (pc->getUser1() == otherUser && pc->getUser2() == currentUser)){
                    cout << "Private chat already exists.\n";
                    return;
                }
            }
        }


        Chat* chat = new PrivateChat(currentUser, otherUser);

        chats.push_back(chat);

        cout << "Private chat created successfully.\n";
    }


    void createGroup() {
        // TODO: Implement group creation
        string groupName;
        int num;

        cout << "Enter group name: ";
        cin >> groupName;

        cout << "Enter number of participants: ";
        cin >> num;
        if(num < 2){
            cout << "Group must have at least 2 participants.\n";
            return;
        }
        vector<string> members;

        for(int i = 0; i < num; i++){
            string user;

            cout << "Enter username: ";
            cin >> user;

            if(findUserIndex(user) == -1){
                cout << "User not found. try again\n";
                i--;
                continue;
            }

            bool exists = false;

            for(const string& m : members){
                if(m == user){
                    exists = true;
                    break;
                }
            }

            if(exists){
                cout << "User already added to the group.\n";
                i--;
                continue;
            }

            members.push_back(user);
        }


        string creator = getCurrentUsername();

        Chat* group = new GroupChat(members, groupName, creator);

        chats.push_back(group);

        cout << "Group created successfully.\n";
    }


    void viewChats() const {
        if(chats.empty()){
            cout << "No chats available.\n";
            return;
        }

        string currentUser = getCurrentUsername();
        int counter = 1;

        for(int i = 0; i < chats.size(); i++){
            Chat* chat = chats[i];

            PrivateChat* pc = dynamic_cast<PrivateChat*>(chat);
            if(pc != nullptr){
                if(pc->getUser1() == currentUser || pc->getUser2() == currentUser){
                    cout << "\nChat " << counter++ << endl;
                    chat->displayChat();
            }
            continue;
        }

        GroupChat* gc = dynamic_cast<GroupChat*>(chat);
        if(gc != nullptr){
            if(gc->isParticipant(currentUser)){
                cout << "\nChat " << counter++ << endl;
                chat->displayChat();
            }
        }
    }
}


    void logout() {
        // TODO: Implement logout
        if(currentUserIndex != -1){
            users[currentUserIndex].setStatus("Offline");
            users[currentUserIndex].updateLastSeen();
        }

        currentUserIndex = -1;

        cout << "Logged out successfully.\n";
    }

    void sendMessage() {

        if(chats.empty()){
            cout << "No chats available.\n";
            return;
        }

        string currentUser = getCurrentUsername();

        vector<int> availableChats;
        int counter = 1;

        cout << "\nYour Chats:\n";

        for(int i = 0; i < chats.size(); i++){

            PrivateChat* pc = dynamic_cast<PrivateChat*>(chats[i]);
            if(pc != nullptr){
                if(pc->getUser1() == currentUser || pc->getUser2() == currentUser){
                    cout << counter << ". ";
                    chats[i]->displayChat();
                    availableChats.push_back(i);
                    counter++;
                }
                continue;
            }

            GroupChat* gc = dynamic_cast<GroupChat*>(chats[i]);
            if(gc != nullptr){
                if(gc->isParticipant(currentUser)){
                    cout << counter << ". ";
                    chats[i]->displayChat();
                    availableChats.push_back(i);
                    counter++;
                }
            }
        }

        if(availableChats.empty()){
            cout << "You are not part of any chats.\n";
            return;
        }

        int choice;
        cout << "\nSelect chat number: ";
        cin >> choice;

        if(choice < 1 || choice > availableChats.size()){
            cout << "Invalid choice.\n";
            return;
        }

        cin.ignore();

        string text;
        cout << "Enter message: ";
        getline(cin, text);

        Message msg(currentUser, text);

        chats[availableChats[choice-1]]->addMessage(msg);

        cout << "Message sent successfully.\n";
    }

    void replyMessage() {

        string currentUser = getCurrentUsername();

        if(chats.empty()){
            cout << "No chats available.\n";
            return;
        }

        int chatIndex;

        cout << "Select chat index: ";
        cin >> chatIndex;

        if(chatIndex < 0 || chatIndex >= chats.size()){
            cout << "Invalid chat.\n";
            return;
        }

        Chat* chat = chats[chatIndex];

        vector<Message>& msgs = chat->getMessages();

        if(msgs.empty()){
            cout << "No messages to reply to.\n";
            return;
        }

        cout << "\nMessages:\n";
        for(int i = 0; i < msgs.size(); i++){
            cout << i << ". ";
            msgs[i].display();
        }

        int msgIndex;
        cout << "Choose message to reply to: ";
        cin >> msgIndex;

        if(msgIndex < 0 || msgIndex >= msgs.size()){
            cout << "Invalid message.\n";
            return;
        }

        cin.ignore();

        string text;
        cout << "Enter your reply: ";
        getline(cin, text);

        Message reply(currentUser, text);

        reply.setReplyTo(&msgs[msgIndex]);

        chat->addMessage(reply);

        cout << "Reply sent.\n";
}
        void searchInChat() {

            if(chats.empty()){
                cout << "No chats available.\n";
                return;
            }

            string currentUser = getCurrentUsername();

            cout << "\nEnter keyword to search: ";
            string keyword;
            cin >> keyword;

            bool found = false;

            for(Chat* chat : chats){

            PrivateChat* pc = dynamic_cast<PrivateChat*>(chat);
                if(pc && (pc->getUser1() == currentUser || pc->getUser2() == currentUser)){

                    vector<Message> results = chat->searchMessages(keyword);

                    if(!results.empty()){
                        cout << "\nMessages found in chat:\n";
                        for(const Message& m : results){
                            m.display();
                        }
                        found = true;
                    }
                }

                GroupChat* gc = dynamic_cast<GroupChat*>(chat);
                if(gc && gc->isParticipant(currentUser)){

                    vector<Message> results = chat->searchMessages(keyword);

                    if(!results.empty()){
                        cout << "\nMessages found in group:\n";
                        for(const Message& m : results){
                            m.display();
                        }
                        found = true;
                    }
                }
            }

            if(!found){
                cout << "No messages found containing this keyword.\n";
            }
        }
        void showParticipants() {

            if(chats.empty()){
                cout << "No chats available.\n";
                return;
            }

            int index;
            cout << "Enter chat index: ";
            cin >> index;

            if(index < 0 || index >= chats.size()){
                cout << "Invalid chat.\n";
                return;
            }

            GroupChat* gc = dynamic_cast<GroupChat*>(chats[index]);

            if(gc == nullptr){
                cout << "This is not a group chat.\n";
                return;
            }

            cout << "\nParticipants:\n";
            gc->displayChat();
        }

        void showAdmins() {

            if(chats.empty()){
                cout << "No chats available.\n";
                return;
            }

            int index;
            cout << "Enter chat index: ";
            cin >> index;

            if(index < 0 || index >= chats.size()){
                cout << "Invalid chat.\n";
                return;
            }

            GroupChat* gc = dynamic_cast<GroupChat*>(chats[index]);

            if(gc == nullptr){
                cout << "This is not a group chat.\n";
                return;
            }

            gc->displayChat();
        }

        void addUserToGroup() {

            int index;
            cout << "Enter group index: ";
            cin >> index;

            if(index < 0 || index >= chats.size()){
                cout << "Invalid group.\n";
                return;
            }

            GroupChat* gc = dynamic_cast<GroupChat*>(chats[index]);

            if(gc == nullptr){
                cout << "This is not a group chat.\n";
                return;
            }

            string user;
            cout << "Enter username to add: ";
            cin >> user;

            gc->addParticipant(getCurrentUsername(), user);
        }

        void promoteToAdmin() {

            int index;
            cout << "Enter group index: ";
            cin >> index;

            GroupChat* gc = dynamic_cast<GroupChat*>(chats[index]);

            if(gc == nullptr){
                cout << "Not a group chat.\n";
                return;
            }

            string user;
            cout << "Enter username: ";
            cin >> user;

            gc->makeAdmin(getCurrentUsername(), user);
        }
        void removeUserFromGroup() {

            int index;
            cout << "Enter group index: ";
            cin >> index;

            if(index < 0 || index >= chats.size()){
                cout << "Invalid chat index.\n";
                return;
            }

            GroupChat* gc = dynamic_cast<GroupChat*>(chats[index]);

            if(gc == nullptr){
                cout << "Not a group chat.\n";
                return;
            }

            string user;
            cout << "Enter username to remove: ";
            cin >> user;

            bool success = gc->removeParticipant(getCurrentUsername(), user);

            if(success){
                cout << user << " has been removed from the group.\n";
            }
            else{
                cout << "Failed to remove user. Make sure you are an admin and the user exists.\n";
            }
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
                    int choice;
cout << "\n1. Start Private Chat\n"
     << "2. Create Group\n"
     << "3. View Chats\n"
     << "4. Send Message\n"
     << "5. Reply Message\n"
     << "6. searchInChat\n"
     << "7. Show Participants\n"
     << "8. Show Admins\n"
     << "9. Add User to Group\n"
     << "10. Promote to Admin\n"
     << "11. Remve User from group\n"
     << "12. Logout\nChoice: ";
     cin >> choice;

                if (choice == 1) startPrivateChat();
                else if (choice == 2) createGroup();
                else if (choice == 3) viewChats();
                else if (choice == 4) sendMessage();
                else if (choice == 5) replyMessage();
                else if (choice == 6) searchInChat();
                else if (choice == 7) showParticipants();
                else if (choice == 8) showAdmins();
                else if (choice == 9) addUserToGroup();
                else if (choice == 10) promoteToAdmin();
                else if (choice == 11) removeUserFromGroup();
                else if (choice == 12) logout();
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
