class Memory {
public:

	Memory() {};
	~Memory() {};

private:

	unsigned int id;
	char hash[256];
	unsigned int *data;

	void CalculateHash();
};

class MemoryManager {
public:

	MemoryManager();
	~MemoryManager();

private:

};