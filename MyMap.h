// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#ifndef MYMAP
#define MYMAP

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap()
	{
		root = nullptr; //creates an empty binary tree
		m_size = 0;
	}

	~MyMap()
	{
		clear(); //responsible for deallocating all items in the map
	}

	void clear()
	{
		eraseTree(root); //calls the recursive deletion function eraseTree with the root as the location to begin deleting nodes
	}
	int size() const { return m_size; } //returns the number of objects in the map
	void associate(const KeyType& key, const ValueType& value) //associates a KeyType with a ValueType by calling the recursive function insert for the binary tree
	{
		insert(key, value, root);
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const //returns a constant pointer to the ValueType object associated with KeyType key, nullptr if no such item found
	{												//calls the recursive function findHelper which returns such a pointer
		return findHelper(key, root);
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;
private:
	struct Node
	{
	public:
		Node(KeyType key, ValueType value)
		{
			m_key = key;
			m_value = value;
			right = left = nullptr;
		}
		KeyType m_key;
		ValueType m_value;
		Node *right, *left;
	};
	int m_size;
	Node *root;
	void eraseTree(Node*& current) //recursive function to erase the contents of your tree
	{
		if (current == nullptr) //calls itself recursively until a nullptr reached
			return;
		eraseTree(current->left); //begins erasing of left subtree
		eraseTree(current->right); //begins erasing of right subtree
		delete current; //deallocate current node
	}
	void insert(const KeyType& key, const ValueType& value, Node*& current)
	{
		if (current == nullptr)
		{
			Node *temp = new Node(key, value); //allocate a new node with the given key and value
			current = temp;
			++m_size;
			return;
		}
		else if (current->m_key == key) //if there already exists a value with an identical key
		{
			current->m_value = value; //update the value mapped to by that key and return
			return;
		}
		else if (current->m_key > key) //if the key being inserted is less than the current position recursively call function with the left child node
			insert(key, value, current->left);
		else if (current->m_key < key) //if the key being inserted is greater than the current position recursively call function with the right child node
			insert(key, value, current->right);
	}

	const ValueType* findHelper(const KeyType& key, const Node* current) const
	{
		if (!current) //if a nullptr reached no such key exists in the tree return nullptr
			return nullptr;
		else if (current->m_key == key) //if find successful, return a pointer to the value stored in that node
			return &(current->m_value);
		else if (current->m_key > key) //if the key being inserted is less than the current position recursively call function with the left child node
			return(findHelper(key, current->left)); 
		else
			return(findHelper(key, current->right)); //if the key being inserted is greater than the current position recursively call function with the right child node
	}
};
#endif // !MYMAP
