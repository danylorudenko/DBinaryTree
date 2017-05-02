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

		BinaryNode<T>&	operator=(const BinaryNode<T>& rhs)
		{
			_entry = rhs.entry_;
		}

		BinaryNode<T>&	operator=(BinaryNode<T>&& rhs)
		{
			_entry = std::move(rhs.entry_);
			lesser_ = rhs.lesser_;	rhs.lesser_ = nullptr;
			greater_ = rhs.greater_;	rhs.greater_ = nullptr;
		}

		bool operator<(const T& rhs)
		{
			return entry_ < rhs;
		}

		BinaryNode<T>* put(const T& new_entry)
		{
			if (entry_ <= new_entry) {
				return put_greater(new_entry);
			}
			else {
				return put_lesser(new_entry);
			}
		}

		BinaryNode<T>* put(T&& new_entry)
		{
			if (entry_ <= new_entry) {
				return put_greater(std::move(new_entry));
			}
			else {
				return put_lesser(std::move(new_entry));
			}
		}

		T& get_entry()
		{
			return entry_;
		}

		bool try_lesser()
		{
			return (lesser_ == nullptr) ? false : true;
		}

		bool try_greater()
		{
			return (greater_ == nullptr) ? false : true;
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

	DBinaryTree<T>& insert(const T& new_entry)
	{
		if (root_ == nullptr) {
			root_ = new BinaryNode<T>(new_entry);
			start_ = root_;
			end_ = root_;
			return *this;
		}
		
		BinaryNode<T>* new_node = root_->put(new_entry);

		if (new_node->get_entry() < start_->get_entry()) {
			start_ = new_node;
		}
		else if(!(new_node->get_entry() < end_->get_entry())) {
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

		if (new_node->get_entry() < start_->get_entry()) {
			start_ = new_node;
		}
		else if (!(new_node->get_entry() < end_->get_entry())) {
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
		BinaryNode<T>* target_root = nullptr;
		BinaryNode<T>& target_node = get_node(target, *root_, &target_root);

		remove_internal(target_node, target_root);
	}

private:

	BinaryNode<T>& find_smallest(BinaryNode<T>& root, BinaryNode<T>*& )
	{
		return (root.try_lesser()) ? find_smallest(*root.lesser_) : root;
	}

	void remove_internal(BinaryNode<T>& target_node, BinaryNode<T>*& target_root)
	{
		if (target_root == nullptr) {
			delete root_;
			root_ = nullptr;
			return;
		}

		if (target_node.try_greater() && target_node.try_lesser()) {
			BinaryNode<T>& smallest_on_right = find_smallest(target_node);
			target_node = smallest_on_right;
			remove_internal(smallest_on_right);
		}
		else if (target_node.try_greater()) {
			auto temp = target_node.greater_;
			delete target_root->greater_;
			target_root->greater_ = temp;
		}
		else if (target_node.try_lesser()) {
			auto temp = target_node.lesser_;
			delete target_root->lesser_;
			target_root->lesser_ = temp;
		}
		else {
			if (target_root->lesser_->entry_ == target_node.entry_) {
				delete target_root->lesser_;
				target_root->lesser_ = nullptr;
			}
			else {
				delete target_root->greater_;
				target_root->greater_ = nullptr;
			}
		}
	}

	BinaryNode<T>& get_node(const T& target, BinaryNode<T>& root, BinaryNode<T>** previous_node)
	{
		if (target < start_->get_entry()) {
			throw std::out_of_range("No such entry in the binary tree.");
		}
		else if (!(target < end_->get_entry())) {
			throw std::out_of_range("No such entry in the binary tree.");
		}

		return get_next(target, root, previous_node);
	}

	void remove_node(BinaryNode<T> target_node)
	{

	}

	// Recursive step into a tree with an attempt to find a target node
	BinaryNode<T>& get_next(const T& target, BinaryNode<T>& current_node, BinaryNode<T>** previous_node)
	{
		if (current_node.entry_ == target) {
			return current_node;
		}
		
		if (target < current_node.entry_) {
			if (!current_node.try_lesser())
			{
				throw std::out_of_range("No such entry in the binary tree.");
			}
			*previous_node = &current_node;
			return get_next(target, *current_node.lesser_, previous_node);
		}
		else {
			if (!current_node.try_greater())
			{
				throw std::out_of_range("No such entry in the binary tree.");
			}
			*previous_node = &current_node;
			return get_next(target, *current_node.greater_, previous_node);
		}
	}

	BinaryNode<T>*			root_		= nullptr;
	BinaryNode<T>*			start_		= nullptr;
	BinaryNode<T>*			end_		= nullptr;

};

#endif