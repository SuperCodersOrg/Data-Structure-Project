# Data Structure Library - Design Proposal
**Project:** Project 01 – Collections Library & Redis Lite

---

# Section 1 – Public API

This project consists of four main components: DynamicArray, LinkedList, HashMap, and Redis Lite. The API is designed to provide the basic operations required for each data structure while keeping the interface easy to understand and maintain.

## DynamicArray/Vector
The DynamicArray (or Vector) is a resizable array that allows for efficient access and modification

```cpp
class Vector {
    private:
        T* data; //Pointer to the array
        size_t capacity; //Total capacity of the array
        size_t  size; //Current number of elements in the array

        void resize(); //Resize the array when capacity is reached
    public:
        // Rule of 3
        Vector(); //Constructor
        Vector(const Vector& other); //Copy Constructor
        Vector& operator=(const Vector& other); //Copy Assignment Operator
        ~Vector(); //Destructor

        Vector(std::initializer_list<T> initList); //Initializer List Constructor

        void push_back(const T& value); //Add an element to the end
        void pop_back(); //Remove the last element;
        void insert(size_t index, const T& value); //Insert an element at a specific index
        void remove(size_t index); //Remove an element at a specific index

        T& operator[](size_t index); //Access an element at a specific index
        T& at(size_t index); //Access an element at a specific index with bounds checking

        T& front(); //Access the first element
        T& back(); //Access the last element

        bool empty() const; //Check if the Vector is empty
        size_t size() const; //Get the current number of elements
        size_t capacity() const; //Get the total capacity of the Vector

        size_t find(const T& value) const; //Find the index of a specific value
        void clear(); //Remove all elements from the Vector
        void reserve(size_t newCapacity); //Reserve memory for a specific capacity
        void shrink_to_fit(); //Reduce capacity to fit the current size
        bool contains(const T& value) const; //Check if the Vector contains a specific value
};
```

## LinkedList
The LinkedList is a linear data structure where each element (node) contains a reference (link) to the next node in the sequence.

```cpp
class List{
    public:
        struct Node {
            T data; //Data stored in the node
            Node *prev; //Pointer to the previous node
            Node *next; //Pointer to the next node

            Node(const T& value) : data(value), prev(nullptr), next(nullptr) {} //Node constructor
        };
    private:
        Node* head; //Pointer to the first node
        Node* tail; //Pointer to the last node
        size_t listSize; //Current number of nodes in the list

        void copyFrom(const List& other); //Copy the contents from another list
    public:
        // Rule of 3
        List(); //Constructor
        List(const List& other); //Copy Constructor
        List& operator=(const List& other); //Copy Assignment Operator
        ~List(); //Destructor

        List(std::initializer_list<T> initList); //Initializer List Constructor

        void push_front(const T& value); //Add an element to the front
        void push_back(const T& value); //Add an element to the back
        void insert(size_t index, const T& value); //Insert an element at a specific index

        void pop_front(); //Remove the first element
        void pop_back(); //Remove the last element

        T& front(); //Access the first element
        T& back(); //Access the last element
        T& at(size_t index); //Access an element at a specific index

        bool empty() const; //Check if the list is empty
        size_t size() const; //Get the current number of nodes in the list

        void clear(); //Remove all elements from the list
        bool contains(const T& value) const; //Check if the list contains a specific value
        Node* find(const T& value); //Find the first node containing a specific value
        void reverse(); //Reverse the order of the nodes in the list

};
```

## HashMap
The HashMap is a data structure that stores key-value pairs and allows for efficient retrieval of values based on their keys.

```cpp
class HashMap {
    private:
        struct Node {
            K key; //Key of the key-value pair
            V value; //Value of the key-value pair
            Node* next; //Pointer to the next node in case of collision

            Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {} //Node constructor
        };

        Node** buckets; //Array of pointers to the head nodes of the linked lists (buckets)

        size_t bucketCount; //Total number of buckets
        size_t elementCount; //Current number of key-value pairs in the HashMap

        size_t hash(const K& key) const; //Hash function to compute the index for a given key
        void rehash(); //Rehash the HashMap when the load factor exceeds a certain threshold (e.g., 0.75)
        void copyFrom(const HashMap& other); //Copy the contents from another HashMap

    public:
        //Rule of 3
        HashMap(); //Constructor
        HashMap(const HashMap& other); //Copy Constructor
        HashMap& operator=(const HashMap& other); //Copy Assignment Operator
        ~HashMap(); //Destructor

        void set(const K& key, const V& value); //Insert or update a key-value pair
        V get(const K& key) const; //Retrieve the value associated with a key
        void remove(const K& key); //Remove a key-value pair by key

        bool contains(const K& key) const; //Check if a key exists in the HashMap
        size_t size() const; //Get the current number of key-value pairs in the HashMap
        float loadFactor() const; //Get the current load factor of the HashMap
        void clear(); //Remove all key-value pairs from the HashMap
        bool empty() const; //Check if the HashMap is empty
};

```
## Redis Lite
The Redis Lite is a simplified version of the Redis key-value store, designed to provide basic functionality for storing and retrieving data.

```cpp
class RedisLite {
    private:
        HashMap<std::string, std::string> dataStore; //Underlying data structure to store key-value pairs
    public:
        void run(); //Start the Redis Lite in the command line interface
};
```

---
# Section 2 - Internal Design
## DynamicArray / Vector

The Vector stores elements inside a contiguous block of heap memory. The class maintains three private members: a pointer to the allocated array (`data`), the current number of stored elements (`size`), and the allocated capacity (`capacity`).

The initial capacity is four elements. Whenever `size == capacity`, the Vector allocates a new array with twice the previous capacity, copies all existing elements, releases the old memory using `delete[]`, and updates the internal pointer.

### Memory Management

The destructor releases the heap array using `delete[]`.

The copy constructor allocates a new array and copies each element individually.

The assignment operator first frees existing memory, allocates new storage, copies every element, and returns `*this`.

This design ensures deep copying and prevents double-free errors.

---
## LinkedList

The LinkedList is implemented as a doubly linked list, where each node contains a pointer to the previous and next nodes. The class maintains pointers to the head and tail nodes, as well as the current size of the list.

The LinkedList supports insertion and deletion at both ends, as well as at specific indices. The `insert` and `remove` methods traverse the list to find the appropriate position for the operation.

### Memory Management

The destructor iterates through the list, deleting each node to free memory. The copy constructor creates a new list by iterating through the source list and copying each node's data. The assignment operator first clears the existing list, then copies nodes from the source list.

This design ensures that each LinkedList instance manages its own memory, preventing memory leaks and dangling pointers.

The LinkedList class also includes a `clear` method that removes all nodes from the list, resetting the size to zero and freeing memory.

---
## HashMap

The HashMap uses an array of pointers to linked lists (buckets) to handle collisions. Each bucket contains a linked list of nodes, where each node stores a key-value pair. The class maintains the number of buckets and the current number of elements.

The `hash` function computes the index for a given key using a simple modulus operation. When the load factor (number of elements divided by number of buckets) exceeds a threshold (e.g., 0.75), the `rehash` method is called to create a new array of buckets with double the previous size, and all existing key-value pairs are reinserted into the new buckets.

### Memory Management
The destructor iterates through each bucket, deleting all nodes in the linked lists and freeing the array of buckets. The copy constructor creates a new HashMap by allocating a new array of buckets and copying each key-value pair from the source HashMap. The assignment operator first clears the existing HashMap, then copies key-value pairs from the source.

This design ensures that each HashMap instance manages its own memory, preventing memory leaks and dangling pointers.

The HashMap class also includes a `clear` method that removes all key-value pairs, resets the size to zero, and frees memory.

---
## Redis Lite
It is a simplified version of the Redis key-value store, designed to provide basic functionality for storing and retrieving data. It uses the HashMap class as its underlying data structure to manage key-value pairs.

It provides a command-line interface for users to interact with the data store, allowing them to set, get, and delete key-value pairs. The `run` method starts the Redis Lite application and listens for user commands.

---
## Rule of Three
Every class owns heap memory will implement:
- Destructor: To free the allocated memory when the object is destroyed.
- Copy Constructor: To create a new object as a copy of an existing object, ensuring deep copying of heap memory.
- Copy Assignment Operator: To assign the contents of one object to another, ensuring deep copying and proper memory management.

Copying and assignment operations will be handled carefully to avoid memory leaks and dangling pointers, ensuring that each object manages its own memory correctly.

---

# Section 3 – Complexity Analysis

This section explains the expected time complexity of the main operations provided by each data structure. The estimates are based on the chosen implementations and represent the average or worst-case performance where applicable.

---

## DynamicArray / Vector

The Vector stores elements in a contiguous block of memory, allowing direct access using an index. Most operations are efficient, but inserting or removing elements in the middle requires shifting existing elements.

| Operation | Time Complexity | Explanation |
|-----------|-----------------|-------------|
| `push_back()` | O(1) amortized | Appending to the end is usually constant time. Occasionally the array must resize, making one operation O(n), but the average remains O(1). |
| `pop_back()` | O(1) | The last element is removed without shifting other elements. |
| `insert(index)` | O(n) | Elements after the insertion point must be shifted one position to the right. |
| `remove(index)` | O(n) | Elements after the removed element must be shifted left to fill the gap. |
| `operator[]` | O(1) | Direct indexing allows immediate access to any element. |
| `at(index)` | O(1) | Performs direct indexing with an additional bounds check. |
| `front()` | O(1) | Returns the first element directly. |
| `back()` | O(1) | Returns the last element directly. |
| `find()` | O(n) | A linear search is required because the elements are not sorted. |
| `size()` | O(1) | Returns the stored size variable. |
| `capacity()` | O(1) | Returns the stored capacity variable. |
| `empty()` | O(1) | Checks whether the size is zero. |
| `clear()` | O(1) | Each element is destroyed, and the size is reset to zero. |
| `reserve(newCapacity)` | O(n) | Allocates a new array and copies existing elements if the new capacity is greater than the current capacity. |
| `shrink_to_fit()` | O(n) | Allocates a new array with size equal to the current number of elements and copies existing elements. |
| `contains()` | O(n) | A linear search is required to check for the presence of a specific value. |

---

## LinkedList

The LinkedList stores each element in a separate node connected using pointers. It allows efficient insertion and deletion at the front and back of the list but requires traversal when accessing a specific position.

| Operation | Time Complexity | Explanation |
|-----------|-----------------|-------------|
| `push_front()` | O(1) | Inserts a new node at the beginning by updating the head pointer. |
| `push_back()` | O(1) | Inserts a new node at the end using the tail pointer. |
| `pop_front()` | O(1) | Removes the first node by moving the head pointer. |
| `pop_back()` | O(1) | Removes the last node by updating the tail pointer. |
| `insert(index)` | O(n) | The list must be traversed to reach the desired position before inserting. |
| `at(index)` | O(n) | Accessing an element requires traversing the list from the head or tail. |
| `front()` | O(1) | Returns the first node's value. |
| `back()` | O(1) | Returns the last node's value. |
| `clear()` | O(n) | Every node must be deleted individually. |
| `size()` | O(1) | Returns the stored size variable. |
| `empty()` | O(1) | Checks whether the list contains any nodes. |
| `contains()` | O(n) | A linear search is required to check for the presence of a specific value. |
| `find()` | O(n) | Traverses the list to locate the first node containing the specified value. |
| `reverse()` | O(n) | Iterates through the list, reversing the direction of the pointers for each node. |

---

## HashMap

The HashMap stores key-value pairs using an array of buckets. Collisions are handled using separate chaining, where each bucket stores a linked list of entries.

| Operation | Average Case | Worst Case | Explanation |
|-----------|--------------|------------|-------------|
| `set()` | O(1) | O(n) | Normally inserts into one bucket. If many collisions occur, traversal of the bucket is required. |
| `get()` | O(1) | O(n) | Retrieves the value directly from the correct bucket in most cases. |
| `remove()` | O(1) | O(n) | Removes the matching key from its bucket. |
| `contains()` | O(1) | O(n) | Searches only the bucket where the key should exist. |
| `loadFactor()` | O(1) | O(1) | Calculated using the stored element count and bucket count. |
| `size()` | O(1) | O(1) | Returns the current number of stored key-value pairs. |
| `clear()` | O(n) | O(n) | Every node in every bucket must be deleted. |
| `rehash()` | O(n) | O(n) | Every key-value pair must be reinserted into the new bucket array. |
| `empty()` | O(1) | O(1) | Checks whether the element count is zero. |

---

## Redis Lite

Redis Lite uses the HashMap class as its underlying storage, so the performance of its commands depends on the HashMap operations.

| Command | Time Complexity | Explanation |
|---------|-----------------|-------------|
| `SET` | O(1) average | Inserts or updates a key-value pair using the HashMap. |
| `GET` | O(1) average | Retrieves the value associated with a key. |
| `DEL` | O(1) average | Removes a key-value pair from the HashMap. |
| `EXISTS` | O(1) average | Checks whether a key exists. |
| `CLEAR` | O(n) | Removes every stored key-value pair. |
| `EXIT` | O(1) | Terminates the application without processing stored data. |

---

## Summary

The selected data structures provide a balance between performance and simplicity. The Vector offers fast random access and efficient appending, making it suitable for sequential storage. The LinkedList provides efficient insertion and deletion at both ends without shifting elements. The HashMap delivers fast average-case lookup, insertion, and removal, making it a good choice for implementing Redis Lite. Together, these data structures satisfy the functional requirements of the project while maintaining efficient memory management through the Rule of Three.

---
# Section 4 – Design Decisions

This section explains the key design choices made for each data structure. Since several implementation details are left to the student, these decisions are documented here along with the reasoning behind them.

---

## DynamicArray / Vector

### Resizing Strategy

The `Vector` starts with an initial capacity of **4** elements. When the array becomes full (`size == capacity`), a new array with **double the previous capacity** is allocated. All existing elements are copied to the new array, the old array is released using `delete[]`, and the internal pointer is updated.

I chose the doubling strategy because it significantly reduces the number of memory reallocations compared to increasing the capacity by one element at a time. Although resizing is an O(n) operation, it happens infrequently, allowing `push_back()` to achieve amortized O(1) performance.

The capacity **does not automatically shrink** when elements are removed. Constantly shrinking and expanding the array could lead to unnecessary memory allocations and reduced performance if elements are frequently inserted and removed. Keeping the allocated memory improves efficiency for workloads where the container continues to grow after deletions.

---

### Error Handling

The `operator[]` function provides direct access without checking whether the index is valid. This is consistent with the behavior of the C++ Standard Library and offers the best performance.

The `at()` function performs bounds checking before accessing an element. If the supplied index is outside the valid range, it throws a `std::out_of_range` exception. This gives users a safer alternative whenever index validation is required.

---

## LinkedList

### List Structure

The LinkedList is implemented as a **doubly linked list**. Every node stores both a pointer to the previous node and a pointer to the next node.

Although this requires one additional pointer per node compared to a singly linked list, it simplifies insertion and deletion because neighboring nodes can be updated without searching for the previous node.

Maintaining both `head` and `tail` pointers also allows insertion and removal at either end of the list in constant time.

---

### Memory Management

Each node is allocated individually using `new`.

The destructor traverses the list from the head node to the end, deleting every node one by one. This guarantees that all dynamically allocated memory is released before the object is destroyed.

When deleting a node from the middle of the list, the previous node's `next` pointer and the next node's `prev` pointer are updated before deleting the current node. This preserves the structure of the list and prevents dangling pointers.

The copy constructor and copy assignment operator create entirely new nodes instead of copying node addresses. Each list therefore owns its own memory independently.

---

## HashMap

### Collision Strategy

The HashMap uses **separate chaining** to resolve collisions.

Each bucket stores the head of a linked list containing all key-value pairs that hash to the same bucket.

I selected separate chaining because it is straightforward to implement and works well even when collisions occur. It also allows insertion and deletion without shifting existing elements, making the implementation simpler than linear probing.

Although separate chaining requires additional memory for linked list nodes, it maintains good performance while keeping the code easier to understand and maintain.

---

### Load Factor and Rehashing

The HashMap begins with a fixed number of buckets.

Whenever the load factor exceeds **0.75**, the HashMap performs a rehash.

During rehashing:

1. A new bucket array with **twice the previous number of buckets** is allocated.
2. Every existing key-value pair is rehashed using the updated bucket count.
3. Each key-value pair is inserted into its new bucket.
4. The old bucket array and linked lists are released.

Doubling the bucket count keeps the average number of elements per bucket low and helps maintain fast lookup performance.

---

### Missing Keys

If `get()` is called with a missing key, the implementation may throw an exception or return a default-constructed value depending on the final implementation.

This behavior keeps the interface simple and avoids unnecessary exceptions during normal operation. Redis Lite can detect the empty string and display an appropriate message to the user when a key is missing.

---

### Hash Function

The hash function converts a string key into an unsigned integer by processing each character of the key.

Each character contributes to the final hash value, which helps distribute similar keys across different buckets. The final bucket index is calculated using the modulo operator:

```cpp
bucketIndex = hashValue % bucketCount;
```

A good hash function distributes keys evenly across all buckets, reducing collisions and improving lookup performance.

A poor hash function causes many keys to map to the same bucket, increasing the length of collision chains and reducing performance.

---

## Rule of Three

The `Vector`, `LinkedList`, and `HashMap` all manage dynamically allocated memory, so each class follows the Rule of Three.

### Destructor

Every destructor releases all dynamically allocated memory owned by the object.

- The Vector deletes its dynamic array.
- The LinkedList deletes every node.
- The HashMap deletes every node in every bucket before deleting the bucket array itself.

This prevents memory leaks.

---

### Copy Constructor

Each copy constructor performs a **deep copy**.

Instead of copying pointers, new memory is allocated and every element or node is copied individually.

This ensures that the copied object owns completely independent memory.

---

### Copy Assignment Operator

The assignment operator first checks for self-assignment.

If the objects are different, the existing memory is released before allocating new memory and copying every element from the source object.

This prevents memory leaks, dangling pointers, and double-free errors.

---

## Redis Lite

Redis Lite is implemented as a lightweight command-line application that uses the custom HashMap as its underlying storage.

The application supports commands such as `SET`, `GET`, `DEL`, `EXISTS`, `CLEAR`, and `EXIT`.

All data is stored only in memory while the application is running. No file storage or persistence is included because the focus of this project is implementing the underlying data structures rather than building a complete database system.

Using the custom HashMap instead of the C++ Standard Library demonstrates that the data structures developed for this project are capable of supporting a practical application.

---

## Overall Design Philosophy

The overall goal of this project is to build reusable data structures that manage their own memory safely and efficiently.

Each class has a single responsibility:

- The Vector manages contiguous dynamic storage.
- The LinkedList manages dynamically connected nodes.
- The HashMap manages key-value storage using hashing.
- Redis Lite provides a simple interface for interacting with the HashMap.

By separating responsibilities between these components, the implementation becomes easier to test, debug, maintain, and extend. The design also follows object-oriented programming principles by encapsulating data and exposing only the operations required by the user.

---
# Memory Diagrams

## Dynamic Array

### Figure 1: Dynamic Array Memory Diagram
This diagram shows the Vector object stored on the stack and its dynamically allocated array stored on the heap.
<p align="center">
<img src="Memory-Diagrams/DynamicArray-Memory Design.jpeg" width="650">
</p>

---

## Linked List

### Figure 2: Linked List Memory Diagram
This diagram shows the List object stored on the stack and its dynamically allocated nodes stored on the heap. Each node contains a pointer to the previous and next nodes, forming a doubly linked list.
<p align="center">
<img src="Memory-Diagrams/LL-Memory Design.jpeg" width="650">
</p>

---

## Hash Map

### Figure 3: Hash Map Memory Diagram
These two diagrams show the HashMap object stored on the stack and its dynamically allocated buckets and nodes stored on the heap. Each bucket contains a pointer to the head of a linked list of nodes, which store key-value pairs.
<p align="center">
<img src="Memory-Diagrams/HashMap-Memory Design.jpeg" width="650">
</p>

<p align="center">
<img src="Memory-Diagrams/HashMap-Memory Design (2).jpeg" width="650">
</p>