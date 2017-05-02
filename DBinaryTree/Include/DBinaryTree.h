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

		BinaryNode<T>* put(const T&& new_entry)
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

		BinaryNode<T>& get_lesser()
		{
			return *lesser_;
		}

		BinaryNode<T>& get_greater()
		{
			return *greater_;
		}

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

		BinaryNode<T>* put_greater(const T&& new_entry) {
			if (greater_ == nullptr) {
				greater_ = new BinaryNode<T>(std::move(new_entry));
				return greater_;
			}

			return greater_->put(new_entry);
		}

		T						entry_;
		BinaryNode<T>*			lesser_ = nullptr;
		BinaryNode<T>*			greater_ = nullptr;
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
		
		return get_next(target, *root_).get_entry();
	}

private:

	BinaryNode<T>& get_next(const T& target, BinaryNode<T>& current_node)
	{
		if (current_node.get_entry() == target) {
			return current_node;
		}
		
		if (target < current_node.get_entry()) {
			if (!current_node.try_lesser())
			{
				throw std::out_of_range("No such entry in the binary tree.");
			}
			return get_next(target, current_node.get_lesser());
		}
		else {
			if (!current_node.try_greater())
			{
				throw std::out_of_range("No such entry in the binary tree.");
			}
			return get_next(target, current_node.get_greater());
		}
	}

	BinaryNode<T>*			root_		= nullptr;
	BinaryNode<T>*			start_		= nullptr;
	BinaryNode<T>*			end_		= nullptr;

};

#endif