#ifndef __DBINARY_TREE_H__
#define __DBINARY_TREE_H__

#include <utility>
#include <stdexcept>

template<typename T>
class DBinaryTree
{
private:
	template<typename T>
	class BinaryNode
	{
	public:
		BinaryNode<T>(const T& entry)
		{
			entry_ = entry;
		}

		BinaryNode<T>(T&& entry)
		{
			entry_ = std::move(entry);
		}

		BinaryNode<T>(const BinaryNode<T>& rhs)
		{
			entry_ = rhs.entry_;
		}

		BinaryNode<T>(BinaryNode<T>&& rhs)
		{
			entry_ = std::move(rhs.entry_);
			lesser_ = rhs.lesser_;	rhs.lesser_ = nullptr;
			greater_ = rhs.greater_;	rhs.greater_ = nullptr;
		}

		BinaryNode<T>&	operator=(const BinaryNode<T>& rhs) = delete;

		BinaryNode<T>&	operator=(BinaryNode<T>&& rhs) = delete;

		bool operator<(const T& rhs) const
		{
			return entry_ < rhs;
		}

		// Recursive putting of the new entry to the appropriate leaf in the tree
		BinaryNode<T>* put(const T& new_entry)
		{
			if (entry_ <= new_entry) {
				return put_greater(new_entry);
			}
			else {
				return put_lesser(new_entry);
			}
		}

		// Recursive putting of the new entry to the appropriate leaf in the tree
		BinaryNode<T>* put(T&& new_entry)
		{
			if (entry_ <= new_entry) {
				return put_greater(std::move(new_entry));
			}
			else {
				return put_lesser(std::move(new_entry));
			}
		}

		bool try_lesser() const 
		{
			return (lesser_ == nullptr) ? false : true;
		}

		bool try_greater() const
		{
			return (greater_ == nullptr) ? false : true;
		}

		BinaryNode<T>* move_lesser()
		{
			auto moved = lesser_;
			lesser_ = nullptr;
			return moved;
		}

		BinaryNode<T>* move_greater()
		{
			auto moved = greater_;
			greater_ = nullptr;
			return moved;
		}

		// Setting new data node in the passed direction.
		// Returning entry that was previously in the inserted position.
		BinaryNode<T>* set_direction(BinaryNode<T>* data, int direction)
		{
			if (direction > 0) {
				auto prevoius_entry = greater_;
				greater_ = data;
				return prevoius_entry;
			}
			if (direction < 0) {
				auto prevoius_entry = lesser_;
				lesser_ = data;
				return prevoius_entry;
			}
			else {
				return nullptr;
			}
		}

		void delete_direction(int direction)
		{
			if (direction < 0) {
				delete lesser_;
				lesser_ = nullptr;
				return;
			}
			if (direction > 0) {
				delete greater_;
				greater_ = nullptr;
				return;
			}
		}

		void clear_pointers()
		{
			lesser_ = nullptr;
			greater_ = nullptr;
		}

		~BinaryNode()
		{
			delete lesser_;
			delete greater_;
		}

		T						entry_;
		BinaryNode<T>*			lesser_ = nullptr;
		BinaryNode<T>*			greater_ = nullptr;

	private:
		BinaryNode<T>* put_lesser(const T& new_entry) {
			if (lesser_ == nullptr) {
				lesser_ = new BinaryNode<T>(new_entry);
				return lesser_;
			}

			return lesser_->put(new_entry);
		}

		BinaryNode<T>* put_lesser(const T&& new_entry) {
			if (lesser_ == nullptr) {
				lesser_ = new BinaryNode<T>(std::move(new_entry));
				return lesser_;
			}

			return lesser_->put(new_entry);
		}

		BinaryNode<T>* put_greater(const T& new_entry) {
			if (greater_ == nullptr) {
				greater_ = new BinaryNode<T>(new_entry);
				return greater_;
			}

			return greater_->put(new_entry);
		}

		BinaryNode<T>* put_greater(T&& new_entry) {
			if (greater_ == nullptr) {
				greater_ = new BinaryNode<T>(std::move(new_entry));
				return greater_;
			}

			return greater_->put(std::move(new_entry));
		}
	};

public:
	DBinaryTree() = default;

	~DBinaryTree()
	{
		delete root_;
	}

	DBinaryTree<T>& insert(const T& new_entry)
	{
		if (root_ == nullptr) {
			root_ = new BinaryNode<T>(new_entry);
			start_ = root_;
			end_ = root_;
			return *this;
		}
		
		BinaryNode<T>* new_node = root_->put(new_entry);

		if (new_node->entry_ < start_->entry_) {
			start_ = new_node;
		}
		else if(!(new_node->entry_ < end_->entry_)) {
			end_ = new_node;
		}

		return *this;
	}

	DBinaryTree<T>& insert(const T&& new_entry)
	{
		if (root_ == nullptr) {
			root_ = new BinaryNode<T>(std::move(new_entry));
			start_ = root_;
			end_ = root_;
			return *this;
		}

		BinaryNode<T>* new_node = root_->put(std::move(new_entry));

		if (new_node->entry_ < start_->entry_) {
			start_ = new_node;
		}
		else if (!(new_node->entry_ < end_->entry_)) {
			end_ = new_node;
		}

		return *this;
	}

	T& get(const T& target)
	{
		if (target < start_->get_entry()) {
			throw std::out_of_range("No such entry in the binary tree.");
		}
		else if (!(target < end_->get_entry())) {
			throw std::out_of_range("No such entry in the binary tree.");
		}
		
		return get_next(target, *root_, nullptr).get_entry();
	}

	void remove(const T& target)
	{
		BinaryNode<T>* parent_node = nullptr;
		int delete_node_direction = 0;

		// Get node to delete, it's parent and direction from parent
		BinaryNode<T>* delete_node = get_next_and_parent(target, *root_, parent_node, delete_node_direction);
		remove_node(*delete_node, *parent_node, delete_node_direction);
	}

private:

	// Safe removement of node.
	// Notification of parents about the changes and recursive self-call to tide up after removement.
	void remove_node(BinaryNode<T>& to_remove, BinaryNode<T>& parent_node, int delete_node_direction)
	{
		if (to_remove.try_greater() && to_remove.try_lesser()) {
			BinaryNode<T>& lesser_r = *to_remove.lesser_;
			to_remove.entry_ = lesser_r.entry_;
			remove_node(lesser_r, to_remove, -1);
		}
		else if (to_remove.try_greater()) {
			auto junk = parent_node.set_direction(to_remove.move_greater(), delete_node_direction);
			junk->clear_pointers();
			delete junk;
		}
		else if (to_remove.try_lesser()) {
			auto junk = parent_node.set_direction(to_remove.move_lesser(), delete_node_direction);
			junk->clear_pointers();
			delete junk;
		}
		else {
			parent_node.delete_direction(delete_node_direction);
		}
		
	}

	// Recursive step into a tree with an attempt to find a target node
	BinaryNode<T>& get_next(const T& target, BinaryNode<T>& current_node)
	{
		if (current_node.entry_ == target) {
			return current_node;
		}
		
		if (target < current_node.entry_) {
			if (!current_node.try_lesser())
			{
				return nullptr;
			}
			return get_next(target, *current_node.lesser_);
		}
		else {
			if (!current_node.try_greater())
			{
				return nullptr;
			}
			return get_next(target, *current_node.greater_);
		}
	}

	// Recursive step into a tree with an attempt to find a target node
	// Also returning parent pointer by passed reference-to-pointer
	// and the direction in which current node lies in relation it's parent.
	BinaryNode<T>* get_next_and_parent(const T& target, BinaryNode<T>& current_node, BinaryNode<T>*& parent_node, int& target_direction)
	{
		if (current_node.entry_ == target) {
			return &current_node;
		}

		parent_node = &current_node;
		if (target < current_node.entry_) {
			if (!current_node.try_lesser())
			{
				throw std::out_of_range("No such entry in the binary tree.");
			}
			target_direction = -1;
			return get_next_and_parent(target, *current_node.lesser_, parent_node, target_direction);
		}
		else {
			if (!current_node.try_greater())
			{
				throw std::out_of_range("No such entry in the binary tree.");
			}
			target_direction = 1;
			return get_next_and_parent(target, *current_node.greater_, parent_node, target_direction);
		}
	}

	// Finds the most left leaf of the passed root node
	BinaryNode<T>& find_smallest(BinaryNode<T>& root)
	{
		return (root.try_lesser()) ? find_smallest(*root.lesser_) : root;
	}

	BinaryNode<T>*			root_		= nullptr;
	BinaryNode<T>*			start_		= nullptr;
	BinaryNode<T>*			end_		= nullptr;

};

#endif