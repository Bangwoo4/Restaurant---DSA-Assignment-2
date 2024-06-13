
#include "main.h"
int static MAXSIZE = 0;

//last

//HuffmanTree
//========================================================================================
class HuffmanTree{                                                                      //
    public:
        class Node{
        public:
            int weight;
            char ch;
            Node* left;
            Node* right;
            Node(int w, char c = '\0', Node* l = nullptr, Node* r = nullptr) : weight(w), ch(c), left(l), right(r){}
            bool ischar() const{
                if (ch == '\0') return 0;
                return 1;
            }
        };
        Node* root = nullptr;
        ~HuffmanTree(){
            destroyTree(root);
        }
        
        //in
        void temp_print(Node* root){
            if(!root->ischar()) {
                cout << root->weight << "\n";
            }
            else {
                cout << root->ch << "-" << root->weight << "\n";
            }
            return;
        }
        
        void print_charNfreq(Node* root){
            if(!root) return;
            print_charNfreq(root->left);
            temp_print(root);
            print_charNfreq(root->right);
            return;
        }
        
        //destructor
        void destroyTree(Node* root){
            if (root == nullptr) {
                return;
            }
            
            destroyTree(root->left);
            destroyTree(root->right);
            
            delete root;
        }

        //helper functions
        long binaryToDecimal(string n) { 
            string num = n; 
            int dec_value = 0; 
        
            int base = 1; 
        
            int len = num.length(); 
            for (int i = len - 1; i >= 0; i--) { 
                if (num[i] == '1') 
                    dec_value += base; 
                base = base * 2; 
            } 
        
            return dec_value; 
        } 
        
        void assignBinaryValue(vector<string>& storeEncodeData, Node* root, string str = "") {
            if (!root) return;
            else if (!root->ischar()){
                assignBinaryValue(storeEncodeData, root->left, str + "0");
                assignBinaryValue(storeEncodeData, root->right, str + "1");
            }
            else{
                storeEncodeData[root->ch] = str;
            }
            return;
        }
        
        int getResultformBinary(Node* root, string name) {
            if (!root->left && !root->right) return 0;
                
            vector<string> storeEncodeData(256, "");
            //gán giá trị cho mỗi char trong string
            assignBinaryValue(storeEncodeData, root);
            //TEST
            //gán test bằng 10 bin cuối -> reverse
            string test = "";
            for (char ch : name){
                test += storeEncodeData[static_cast<unsigned char>(ch)];
            }
            reverse(test.begin(), test.end());
            if(test.length() > 10){
                string temp = "";
                for (unsigned int i = 0; i < 10; i++){
                    temp += test[i];
                }
                test = temp;
            }
            int result = binaryToDecimal(test);
            return result;
        }
        
        bool isValidName(string name){
            if (name.length() < 3) return 0;
            unordered_set<char> charSet;
            for (char c : name) {
                charSet.insert(c);
            }
            return charSet.size() >= 3;
        }
        
        int getHeight(Node* node) {
            if (!node) return 0;
            return 1 + max(getHeight(node->left), getHeight(node->right));
        }
        
        int getBalance(Node* node){
            return getHeight(node->left) - getHeight(node->right);
        }
        
        Node* rotateRight(Node* node) {
            //cout << "called rotateRight:" << endl;
            Node* newRoot = node->left;
            node->left = newRoot->right;
            newRoot->right = node;
            return newRoot;
        }
        
        Node* rotateLeft(Node* node) {
            //cout << "called rotateLeft:" << endl;
            Node* newRoot = node->right;
            node->right = newRoot->left;
            newRoot->left = node;
            return newRoot;
        }
        
        Node* performBalaneSubtree(Node* node, int& count) {
            int leftHeight = getHeight(node->left);
            int rightHeight = getHeight(node->right);
            int balanceFactor = leftHeight - rightHeight;
            if (balanceFactor > 1){
                if (getHeight(node->left->left) >= getHeight(node->left->right) && count < 3) {
                    // LL case
                    //cout << "LL case: " << endl;
                    count++;
                    return rotateRight(node);
                } 
                else{
                    // LR case
                    //cout << "LR case: " << endl;
                    node->left = rotateLeft(node->left);
                    count++;
                    return rotateRight(node);
                }
            } 
            else if (balanceFactor < -1){
                if (getHeight(node->right->right) >= getHeight(node->right->left) && count < 3) {
                    // RR case
                    //cout << "RR case: " << endl;              
                    count++;
                    return rotateLeft(node);
                } 
                else{
                    // RL case
                    //cout << "RL case: " << endl;              
                    node->right = rotateRight(node->right);
                    count++;
                    return rotateLeft(node);
                }
            }
            return node;
        }
        
        Node* TreeStructureBalancing(Node* node, int depth = 0) {
            if (!node or depth == 1) return node;
            //cout << "count in balanceTree: " << count << endl;
            //if (count >= 3) return node;
            //cân bằng node
            node = performBalaneSubtree(node, depth);
            //cân bằng 2 node con
            //if (count >= 3) return node;
            node->left = TreeStructureBalancing(node->left, depth);
            //if (count >= 3) return node;
            node->right = TreeStructureBalancing(node->right, depth);
            //if (count >= 3) return node;
            return node;
        }

        vector<Node*> createNodes(vector<pair<char, int>>& freq) {
            vector<Node*> HuffmanV;
            for (const auto& p : freq) {
                Node* newNode = new Node(p.second, p.first);
                HuffmanV.push_back(newNode);
            }
            return HuffmanV;
        }
        
        void mergeNodes(vector<Node*>& HuffmanV) {
            while (HuffmanV.size() > 1) {
                int count = 0;
                Node* smallest1 = HuffmanV.back();
                HuffmanV.pop_back();
                Node* smallest2 = HuffmanV.back();
                HuffmanV.pop_back();
                Node* newNode = new Node(smallest1->weight + smallest2->weight, '\0', smallest1, smallest2);
                unsigned int i = 0;
                while (i < 3) {
                    newNode = TreeStructureBalancing(newNode, count);
                    i++;
                }
                unsigned int insertIndex = 0;
                while (insertIndex < HuffmanV.size() && newNode->weight < HuffmanV[insertIndex]->weight) {
                    insertIndex++;
                }
                HuffmanV.insert(HuffmanV.begin() + insertIndex, newNode);
            }
            return;
        }
        
        //method
        vector<pair<char, int>> NameSortingtoPair(string& name){
            if(!isValidName(name)) return {};
            vector<int> count(256, 0);
            vector<pair<char, int>> maps;
            vector<pair<char, int>> encodedPair;
            for (char ch:name){
                count[ch]++;
            }
            
            for (unsigned int i = 0; i < name.length(); i++){
                if(count[name[i]] > 0){
                    maps.push_back(make_pair(static_cast<char>(name[i]), count[name[i]]));
                    count[name[i]] = 0;
                }
            }
            
            for (char ch : name){
                count[ch]++;
            }
            
            //mã hoá name
            for (unsigned int i = 0; i < name.length(); i++){
                if (isupper(name[i])){
                    name[i] = (name[i] + count[name[i]] - 65) % 26 + 65;
                }
                else {
                    name[i] = (name[i] + count[name[i]] - 97) % 26 + 97;
                }
            }
            
            //mã hoá maps
            for(auto& item : maps){
                if(isupper(item.first)){
                    item.first = char(int(item.first + item.second - 65) % 26 + 65);
                }
                else{
                    item.first = char(int(item.first + item.second - 97) % 26 + 97);
                }
            }
            
            //gộp phần tử
            for (auto& item : maps){
                bool found = false;
                for (auto& res : encodedPair){
                    if(res.first == item.first){
                        //cộng w
                        res.second += item.second;
                        found = true;
                        break;
                    }
                }
                if(!found){
                    encodedPair.push_back(item);
                }
            }
            
            auto compare = [](const pair<char, int>& a, const pair<char, int>& b) {
                if (a.second != b.second) {
                    // Sắp xếp theo thứ tự giảm dần tần số
                    return a.second > b.second;
                } else if (islower(a.first) && isupper(b.first)) {
                    // Chữ in hoa sẽ được sắp xếp trước chữ thường
                    return false;
                } else if (isupper(a.first) && islower(b.first)) {
                    // Chữ thường sẽ được sắp xếp sau chữ in hoa
                    return true;
                } else {
                    // Cùng là chữ in hoa hoặc cùng là chữ thường
                    // Chữ nào lớn hơn đứng trước
                    return a.first > b.first;
                }
            };
            sort(encodedPair.begin(), encodedPair.end(), compare);
            return encodedPair;
        }
        
        Node* PairtoHuffmanTree(vector<pair<char, int>>& freq) {
            vector<Node*> HuffmanV = createNodes(freq);
            mergeNodes(HuffmanV);
            return HuffmanV[0];
        }
        
        int encodeName(string name){
            vector<pair<char, int>> pair_freqMap = NameSortingtoPair(name);
            if(!pair_freqMap.empty()){
                destroyTree(root);
                root = PairtoHuffmanTree(pair_freqMap);
                if(!root->left && !root->right) return 0; 
                return getResultformBinary(root, name);
            }
            return -1;
        }
        
        void hand(){
            return print_charNfreq(root);
        }
};
//========================================================================================

//GOJO_RESTAURANT
//========================================================================================
class GOJO_RESTAURANT{                                                                  //
    class BST{
        public:
        class Node{
            public:
            int data;
            Node* left;
            Node* right;
            Node(int result) : data(result), left(nullptr), right(nullptr){}
        };
        Node* root = nullptr;   
        queue<int> q; 
        BST():root(nullptr){}
        ~BST(){
            deleteTree(root);
        }
        
        void deleteTree(Node* node) {
            if (node) {
                deleteTree(node->left);
                deleteTree(node->right);
                delete node;
            }
            return;
        }
        int BSTsize(){ 
            return q.size();
        }
        
        //Helper functions insert/delete customer/print
        Node* insertInBST(Node* root, int result){
            if(!root) {
                q.push(result);
                return new Node(result);
            }
            else if (result >= root->data){
                root->right  = insertInBST(root->right, result);
            }
            else if (result < root->data){
                root->left = insertInBST(root->left, result);
            }
            return root;    
        }
        
        Node* minValueNode(Node* node) {
            Node* current = node;
            while (current && current->left)
                current = current->left;
            return current;
        }
        
        Node* deleteInBST(Node* root,int result){   
            if (!root) return root;
        
            if (root->data > result) {
                root->left = deleteInBST(root->left, result);
                return root;
            }
            else if (root->data < result) {
                root->right = deleteInBST(root->right, result);
                return root;
            }
            //tìm thấy node cần xoá
            else{   
                if (!root->left) {
                    Node* temp = root->right;               
                    delete root;
                    return temp;    
                }
                else if (!root->right) {
                    Node* temp = root->left;
                    delete root;
                    return temp;
                }
                //nếu node cần xoá có 2 con trái phải
                else {
                   Node* temp = minValueNode(root->right);
                   root->data = temp->data;
                   root->right = deleteInBST(root->right, temp->data);   
                }
                return root;
            }
        }
        
        void InoderPrint(Node* root){
            if(!root) return;
            InoderPrint(root->left);
            cout << root->data << "\n";
            InoderPrint(root->right);
            return;
        }
        
        //Functions tính Y
        int treeLength(Node* root) {
            int length = 0;
            if (root != nullptr) {
                length++;
                length += treeLength(root->left);
                length += treeLength(root->right);
            }
            return length;
        }
        
        unsigned long long factorial(unsigned long long num){   
            static std::vector<unsigned long long> cache = {1};  // Memoization cache
            if (num < cache.size())
                return cache[num];
            unsigned long long fact = cache.back();
            for (unsigned int i = cache.size(); i <= num; i++)
            {
                fact *= i;
                cache.push_back(fact);
            }
            return fact;
        }
        
        unsigned long long nCr(unsigned long long r, unsigned long long n) {
            vector<unsigned long long> dp(r + 1, 0);
            dp[0] = 1;
            for (unsigned long long i = 1; i <= n; i++) {
                unsigned long long prev = 1;
                for (unsigned long long j = 1; j <= min(i, r); j++) {
                    unsigned long long temp = dp[j];
                    dp[j] = (prev + dp[j]) % MAXSIZE;
                    prev = temp;
                }
            }
            return dp[r];
        }
        
        unsigned long long calculateY(Node* root) {
            if (!root) return 1;
            unsigned long long leftTreeLength = treeLength(root->left);
            unsigned long long rightTreeLength = treeLength(root->right);
            unsigned long long combinations = nCr(leftTreeLength, leftTreeLength + rightTreeLength);
            unsigned long long result = calculateY(root->left) * calculateY(root->right) % MAXSIZE;
            result = result * combinations % MAXSIZE;
            return result;
        }
        
        //Method cho BST
        void insert(int result){
            root = insertInBST(root, result);
            return;
        }
        
        void remove(int result){
            root = deleteInBST(root, result);
            return;
        }
        
        void kickout_Rokusen(){
            if(q.empty()) return;
            unsigned long long Y = calculateY(root) % MAXSIZE;
            
            for (unsigned int i = 0; i < Y && !q.empty(); i++){  
                int temp = q.front();
                //xoá trong q
                q.pop();
                //xoá trong nhà hàng
                remove(temp);
            }
        }
        
        void print(){
            return InoderPrint(root);
        }
    };
    public:
        vector<BST> areas; //các khu vực | mỗi khu vực là một cây BST
        GOJO_RESTAURANT() : areas(MAXSIZE + 1){}
    //method cho nhà hàng GOJO
        void insert_to_area(int result){
            int ID = result % MAXSIZE + 1;
            areas[ID].insert(result);
            return;
        }
        void Rokusen() {
            int i = 1;
            while (i <= MAXSIZE) {
                areas[i].kickout_Rokusen();
                i++;
            }
            return;
        }
        void Limitless(int n){
            //nếu nums không tồn tại trong nhà hàng hoặc size cây ở khu vực num == 0 -> return
            if(n <= 0 || n > MAXSIZE || areas[n].BSTsize() == 0) return;
            areas[n].print();
            return;
        }
};
//========================================================================================

//SOKUNA_RESTAURANT
//========================================================================================
class SOKUNA_RESTAURANT{                                                                //
    public:
        //khu vực phần tử trong minHeap
        class area{
            public:
                int ID;                 
                list<int> customers;    
                area(int ID) : ID(ID) {}
                int size() const {return customers.size();}
                //thêm khách
                void insert(int result){
                    customers.push_front(result);
                }
                
                //đuổi khách theo FIFO
                void remove(int n){
                    while(n!= 0 && !customers.empty()){
                        cout << customers.back() << "-" <<ID << "\n";
                        customers.pop_back();
                        n--;
                    }
                }
                
                //LIFO
                void print_LIFO(int n) {
                    auto it = customers.rbegin();
                    while (n > 0 && it != customers.rend()) {
                        cout << ID << "-" << *it << "\n";
                        it++;
                        n--;
                    }
                }
                
                //FIFO
                void print_FIFO(int n) {
                    auto it = customers.begin();
                    while (n > 0 && it != customers.end()) {
                        cout << ID << "-" << *it << "\n";
                        it++;
                        n--;
                    }
                }
        };
        vector<area*> minHeap;
        list<area*> LRU; 
        SOKUNA_RESTAURANT(){}
        ~SOKUNA_RESTAURANT() {
            for (area* a : minHeap){
                delete a;
            }
        }
        
        //helper functions
        bool compare(int index1, int index2){
            if (minHeap[index1]->size() < minHeap[index2]->size()){
                return true;
            }
            else if (minHeap[index1]->size() == minHeap[index2]->size()){
                int order1 = 0, order2 = 0;
                for (auto it : LRU){
                    if (it == minHeap[index1]) {
                        break;
                    }
                    ++order1;
                }
                for (auto it : LRU){
                    if (it == minHeap[index2]) {
                        break;
                    }
                    ++order2;
                }
                return order1 > order2;
            }
            return false;
        }
        
		void reHeapUp(int index){
            int parent = (index - 1)/2;
            if (parent >= 0 && compare(index, parent)) {
                swap(minHeap[index], minHeap[parent]);
                reHeapUp(parent);
            }
        }
        
        void reHeapDown(int index){
            unsigned int left = 2 * index + 1;
            unsigned int right = 2 * index + 2;
            int smallest = index;
            if (left < minHeap.size() && compare(left, smallest)) {
                smallest = left;
            }
            if (right < minHeap.size() && compare(right, smallest)) {
                smallest = right;
            }
            if (smallest != index) {
                swap(minHeap[index], minHeap[smallest]);
                reHeapDown(smallest);
            }
        }
        
            //xoá area trong trường hợp không còn khách
        void removeAreaformHeap(std::vector<area*>& minHeap, area* node, int index){
            std::swap(minHeap[index], minHeap.back());
            // xóa khỏi danh sách liên kết
            removeArea(minHeap[minHeap.size() - 1]);
            delete minHeap[minHeap.size() - 1];
            minHeap.pop_back();
        }
        
            //tìm area||thêm area vào đầu LRU
        void addAreaToHeadLRU(area* Area){
            //Tìm Area trong LRU, nếu có thì để lên đầu 
            auto it = find(LRU.begin(), LRU.end(), Area);
            if(it != LRU.end()){
                LRU.splice(LRU.begin(), LRU, it);
            }
            //nếu không thì push Area vào đầu
            else{
                LRU.push_front(Area);
            }
        }
        
		void preorder(unsigned int index, int number){  
            if(index >= minHeap.size()) return;
            minHeap[index]->print_FIFO(number);
            //leftChild
            preorder(index * 2 + 1, number);
            //rightChild
            preorder(index * 2 + 2, number);
        }
        
		void removeArea(area* Area){
            auto it = find(LRU.begin(), LRU.end(), Area);
            if(it != LRU.end()){
                LRU.erase(it);
            }
        }
        
		int findAreaIndex(int ID) {
            for (auto it = minHeap.begin(); it != minHeap.end(); ++it) {
                if ((*it)->ID == ID) {
                    return std::distance(minHeap.begin(), it);
                }
            }
            return -1;
        }
        
        void updateArea(int index, int result){
            minHeap[index]->insert(result);
            addAreaToHeadLRU(minHeap[index]);
            reHeapDown(index);
        }
        
		void createNewArea(int ID, int result){
            area* newArea = new area(ID);
            minHeap.push_back(newArea);
            int index = minHeap.size() - 1;
            minHeap[index]->insert(result);
            addAreaToHeadLRU(minHeap[index]);
            reHeapUp(index);
        }

        queue<area*> getOutAreasList(int n){
            //tạo clone của minHeap
            vector<area*> clone;
            copy(minHeap.begin(), minHeap.end(), back_inserter(clone));
            queue<area*> outAreasList;

            int count = min(n, static_cast<int>(minHeap.size()));
            int i = 0;

            //danh sách area cần xoá
            while (i < count){
                area* areaOut = minHeap.front();
                swap(minHeap[0], minHeap[minHeap.size() - 1]);
                minHeap.pop_back();
                reHeapDown(0);
                outAreasList.push(areaOut);
                i++;
            }
            minHeap = clone;
            return outAreasList;
        }
        
        void processOutArea(area* outArea, int n) {
            outArea->remove(n);
            int index = std::find(minHeap.begin(), minHeap.end(), outArea) - minHeap.begin();
            if (outArea->size() == 0) {
                removeAreaformHeap(minHeap, outArea, index);
            }
            reHeapDown(index);
        }
            
        //method cho Sokuna
        void insertArea(int result) {
            int ID = result % MAXSIZE + 1;
            int index = findAreaIndex(ID);
            if (index != -1){
                updateArea(index, result);
            } 
            else{
                createNewArea(ID, result);
            }
        }
        
		void Keiteiken(int n) {
            //nếu không có khách
            if (minHeap.empty()) return;

            queue<area*> outAreasList = getOutAreasList(n);
            while (!outAreasList.empty()) {
                area* outArea = outAreasList.front();
                outAreasList.pop();
                processOutArea(outArea, n);
            }
            return;
        }
        
        void cleave(int n){
            if (n <= 0) return;
            return preorder(0, n);
        }
};
//========================================================================================

//new Restaurant
//========================================================================================
class Restaurant{                                                                       //
public:
    HuffmanTree huffman;
    GOJO_RESTAURANT gojo;
    SOKUNA_RESTAURANT sokuna;
    
    void LAPSE(string name){
        int result = huffman.encodeName(name);
 
        if(result == -1) return;
        else if(result % 2 == 0){
            //cout << "called 1";
            sokuna.insertArea(result);
        }
        else{
            //cout << "called 2";
            gojo.insert_to_area(result);
        }
        return;
    }   
    void KOKUSEN(){
        //cout << "rokusen";
        return gojo.Rokusen();
    }
    void KEITEIKEN(int num){
        //cout << "keuteiken";
        return sokuna.Keiteiken(num);
    }
    void HAND(){
        return huffman.hand();
    }
    void LIMITLESS(int num){
        return gojo.Limitless(num);
    }
    void CLEAVE(int num){
        return sokuna.cleave(num);
    }
};
//========================================================================================

void simulate(string filename){
    ifstream File(filename);
    if (File.fail()) return;
    
    string name;    
    string str;
    int n;
    File >> str; 
    File >> MAXSIZE;
    Restaurant* newRestaurant = new Restaurant();
    while (File >> str) {
        if (str == "LAPSE") {
            File >> name;
            newRestaurant->LAPSE(name);
        }
        else if (str == "KOKUSEN") {
            newRestaurant->KOKUSEN();
        }
        else if (str == "KEITEIKEN") {
            File >> n;
            newRestaurant->KEITEIKEN(n);
        }
        else if (str == "HAND") {
            newRestaurant->HAND();
        }
        else if (str == "LIMITLESS") {
            File >> n;
            newRestaurant->LIMITLESS(n);
        }
        else if (str == "CLEAVE") {
            File >> n;
            newRestaurant->CLEAVE(n);
        }
    }
    delete newRestaurant;
}