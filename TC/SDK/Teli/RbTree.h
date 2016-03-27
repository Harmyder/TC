#pragma once

#include "SDK\Teli\ContainerUtility.h"

namespace sdk
{
namespace teli
{
	template<class Mytree>
	class TreeConstIterator : public IteratorBase
	{
	public:
		typedef TreeConstIterator<Mytree> Myiter;
		typedef std::bidirectional_iterator_tag iterator_category;

		typedef typename Mytree::Nodeptr Nodeptr;
		typedef typename Mytree::value_type value_type;
		typedef typename Mytree::difference_type difference_type;
		typedef typename Mytree::const_pointer pointer;
		typedef typename Mytree::const_reference reference;

		TreeConstIterator()
			: ptr_(0)
		{
		}

		TreeConstIterator(Nodeptr pnode, const Mytree* plist) : ptr_(pnode)
		{
			Adopt(plist);
		}

		reference operator*() const
		{
			return Mytree::value(ptr_);
		}

		pointer operator->() const
		{
			return &Mytree::value(ptr_);
		}

		Myiter& operator++()
		{
			if (
				GetContainer() == 0 ||
				ptr_ == 0 ||
				Mytree::isnil(ptr_) // end()
				)
			{
				throw "Iterator can't be incremented";
			}
			else if (!Mytree::isnil(Mytree::right(ptr_)))
			{
				ptr_ = Mytree::Min(Mytree::right(ptr_));
			}
			else
			{
				Nodeptr pnode;
				while (!Mytree::isnil(pnode = Mytree::parent(ptr_)) && ptr_ == Mytree::right(pnode))
				{
					ptr_ = pnode;
				}

				ptr_ = pnode;	// ==> parent (head if end())
			}
			return (*this);
		}

		Myiter operator++(int)
		{
			Myiter _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		Myiter& operator--()
		{
			if (GetContainer() == 0 || ptr_ == 0)
			{
				throw "Iterator can't be decremented";
			}

			if (Mytree::isnil(ptr_))
			{
				ptr_ = Mytree::right(ptr_);	// end() ==> rightmost
			}
			else if (!Mytree::isnil(Mytree::left(ptr_)))
			{
				ptr_ = Mytree::Max(Mytree::left(ptr_));
			}
			else
			{
				Nodeptr pnode;
				while (!Mytree::isnil(pnode = Mytree::parent(ptr_)) && ptr_ == Mytree::left(pnode))
				{
					ptr_ = pnode;
				}

				if (Mytree::isnil(ptr_))
				{
				}
				else
				{
					ptr_ = pnode;
				}
			}
			return *this;
		}

		Myiter operator--(int)
		{
			Myiter tmp = *this;
			--*this;
			return tmp;
		}

		bool operator==(const Myiter& right) const
		{
			return (ptr_ == right.ptr_);
		}

		bool operator!=(const Myiter& right) const
		{
			return *this != right;
		}

		Nodeptr Mynode() const
		{
			return ptr_;
		}

		Nodeptr ptr_;
	};

	template<class Mytree>
	class TreeIterator : public TreeConstIterator<Mytree>
	{
	public:
		typedef typename Mytree::pointer pointer;
		typedef typename Mytree::reference reference;

		using TreeConstIterator<Mytree>::TreeConstIterator;
	};

	template <class T>
	class RbTreeNode
	{
		typedef RbTreeNode<T> Me;

	public:
		Me *left;
		Me *right;
		Me *parent;

		char isnil;
		char color;

		T value;
	};

	template <class T, class Pr>
	class RbTreeTraits
	{
	public:
		typedef T key_type;
		typedef T value_type;

		typedef Pr key_compare;

		typedef value_type *pointer;
		typedef const value_type *const_pointer;

		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef RbTreeNode<T> Node;
		typedef RbTreeNode<T> *Nodeptr;
		typedef Nodeptr& Nodepref;

		typedef typename std::allocator<Node> AlNode;
	};

	template <class Traits>
	class RbTreeValue : public ContainerBase
	{
		typedef RbTreeValue<Traits> Me;

	public:
		typedef typename Traits::key_type key_type;
		typedef typename Traits::value_type value_type;

		typedef typename Traits::key_compare key_compare;

		typedef typename Traits::pointer pointer;
		typedef typename Traits::const_pointer const_pointer;

		typedef typename Traits::reference reference;
		typedef typename Traits::const_reference const_reference;

		typedef typename Traits::size_type size_type;
		typedef typename Traits::difference_type difference_type;

		typedef typename Traits::Node Node;
		typedef typename Traits::Nodeptr Nodeptr;
		typedef typename Traits::Nodepref Nodepref;

		RbTreeValue()
		{
			head_ = nullptr;
			size_ = 0;
		}

		enum RedBlack
		{
			RED, BLACK
		};

		static char& color(Nodeptr pnode)
		{
			return pnode->color;
		}

		static char& isnil(Nodeptr pnode)
		{
			return pnode->isnil;
		}

		static Nodepref left(Nodeptr pnode)
		{
			return pnode->left;
		}

		static Nodepref parent(Nodeptr pnode)
		{
			return pnode->parent;
		}

		static Nodepref grandpa(Nodeptr pnode)
		{
			return pnode->parent;
		}

		static Nodepref right(Nodeptr pnode)
		{
			return pnode->right;
		}

		static reference value(Nodeptr pnode)
		{
			return pnode->value;
		}

		static Nodeptr Max(Nodeptr pnode)
		{
			while (!isnil(right(pnode)))
			{
				pnode = right(pnode);
			}
			return pnode;
		}

		static Nodeptr Min(Nodeptr pnode)
		{
			while (!isnil(left(pnode)))
			{
				pnode = left(pnode);
			}
			return pnode;
		}

	protected:
		Nodeptr head_;
		size_type size_;
	};

	template <class Traits>
	class RbTreeAlloc : public RbTreeValue<Traits>
	{
	public:
		RbTreeAlloc()
		{
			head_ = CreateHeadNode();
		}

		~RbTreeAlloc()
		{
			DeleteHeadNode();
		}

		Nodeptr CreateHeadNode()
		{
			Nodeptr pnode = allocator_.allocate(1);

			left(pnode) = pnode;
			parent(pnode) = pnode;
			right(pnode) = pnode;

			color(pnode) = BLACK;
			isnil(pnode) = true;

			return pnode;
		}

		void DeleteHeadNode()
		{
			allocator_.deallocate(head_, 1);
		}

		template <class... Args>
		Nodeptr CreateNode(Args&&... args)
		{
			Nodeptr pnode = allocator_.allocate(1);

			left(pnode) = head_;
			parent(pnode) = head_;
			right(pnode) = head_;

			color(pnode) = RED;
			isnil(pnode) = false;

			allocator_.construct(std::addressof(value(pnode)), std::forward<Args>(args)...);

			return pnode;
		}

		void DeleteNode(Nodeptr pnode)
		{
			allocator_.deallocate(pnode, 1);
		}

	private:
		typename Traits::AlNode allocator_;
	};

	template <class T, class Pr = std::less<T>>
	class RbTree : public RbTreeAlloc<RbTreeTraits<T, Pr>>
	{
	public:
		typedef RbTree<T, Pr> Me;

		typedef TreeConstIterator<Me> const_iterator;
		typedef TreeIterator<Me> iterator;

		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		typedef std::pair<iterator, bool> Pairib;

	public:
		iterator begin()
		{
			return iterator(leftmost(), this);
		}

		const_iterator begin() const
		{
			return const_iterator(leftmost(), this);
		}

		iterator end()
		{
			return iterator(head_, this);
		}

		const_iterator end() const
		{
			return const_iterator(head_, this);
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		const_iterator cbegin() const
		{
			return ((const Me *)this)->begin();
		}

		const_iterator cend() const
		{
			return ((const Me *)this)->end();
		}

		const_reverse_iterator crbegin() const
		{
			return ((const Me *)this)->rbegin();
		}

		const_reverse_iterator crend() const
		{
			return ((const Me *)this)->rend();
		}

		const_iterator Find(const T& value) const;
		iterator Find(const T& value);

		Pairib Insert(value_type&& value)
		{
			return InsertInternal(std::forward<value_type>(value));
		}

		Pairib Insert(const value_type& value)
		{
			return InsertInternal(value);
		}

		size_type size()
		{
			return size_;
		}

		bool empty()
		{
			return size() == 0;
		}

	private:
		template <class Val>
		Pairib InsertInternal(Val&& val)
		{
			bool add_left = true;

			Nodeptr try_node = root();
			Nodeptr parent_node = head_;

			while (!isnil(try_node))
			{
				parent_node = try_node;
				add_left = Comp(val, value(try_node));
				try_node = add_left ? left(try_node) : right(try_node);
			}

			iterator parent = iterator(parent_node, this);
			if (!add_left);
			else if (parent == begin())
			{
				return Pairib(InsertAt(add_left, parent_node, std::forward<Val>(val)), true);
			}
			else
			{
				// As we use '<' for comparison, then we can go right even if value of the node is equal to value to insert,
				// so we need to check if this is the case.
				--parent;
			}

			if (Comp(value(parent.Mynode()), val))
			{
				return Pairib(InsertAt(add_left, parent_node, std::forward<Val>(val)), true);
			}
			else
			{
				return Pairib(parent, false);
			}
		}

		template <class Val>
		iterator InsertAt(const bool kAddLeft, Nodeptr parent_node, Val&& val)
		{
			Nodeptr new_node = CreateNode(std::forward<Val>(val));

			if (parent_node == head_)
			{
				root() = new_node;
				leftmost() = new_node;
				rightmost() = new_node;
			}
			else if (kAddLeft)
			{
				left(parent_node) = new_node;
				if (parent_node == leftmost())
					leftmost() = new_node;
			}
			else
			{
				right(parent_node) = new_node;
				if (parent_node == rightmost())
					rightmost() = new_node;
			}

			++size_;
			new_node->parent = parent_node;

			return iterator(new_node, this);
		}

		void FixupInsertion(Nodeptr new_node)
		{
			Nodeptr node = new_node;
			while ( color(parent(node)) == RED )
			{
				const bool parent_is_left = (parent(node) == left(grandpa(node)));
				if (parent_is_left)
				{
					if (color(right(grandpa(node))) == RED) // Case I
					{
						color(parent(node)) = BLACK;
						color(right(grandpa(node))) = BLACK;
						color(grandpa(node)) = RED;
					}
					else
					{
						if (node == right(parent(node))) // Case II (Zig-Zag)
						{
							node = parent(node);
							LeftRotate(node);
						}
						// Case III
						color(parent(node)) = BLACK;
						color(parent(parent(node))) = RED;
						RightRotate(grandpa(node));
					}
				}
                else
                {
                    if (color(left(grandpa(node))) == RED) // Case I
                    {
                        color(parent(node)) = BLACK;
                        color(left(grandpa(node))) = BLACK;
                        color(grandpa(node)) = RED;
                    }
                    else
                    {
                        if (node == left(parent(node))) // Case II (Zig-Zag)
                        {
                            node = parent(node);
                            RightRotate(node);
                        }
                        // Case III
                        color(parent(node)) = BLACK;
                        color(parent(parent(node))) = RED;
                        LeftRotate(grandpa(node));
                    }
                }
			}
			color(root()) = BLACK;
		}

        iterator erase(const_iterator where)
        {
            if (where->GetContainer() != this || isnil(where->Mynode()))
                throw "Erase iterator outside of the range";
            auto where_next = where + 1;

            Nodeptr erased_node = where.Mynode();

            Nodeptr y = erased_node;
            auto original_color = color(erased_node);

            Nodeptr fix_node;
            if (isnil(left(erazed_node)))
            {
                fix_node = right(erazed_node);
                MoveSubtree(fix_node, erazed_node);
                if (leftmost() == erased_node)
                {
                    leftmost() = isnil(fix_node)
                        ? parent(fix_node)
                        : Min(fix_node);
                }
            }
            else if (isnil(right(erazed_node)))
            {
                fix_node = left(erazed_node);
                MoveSubtree(fix_node, erazed_node);
                if (rightmost() == erased_node)
                {
                    rightmost() = isnil(fix_node)
                        ? parent(fix_node)
                        : Max(fix_node);
                }
            }
            else
            {
                y = Min(right(erased_node));
                original_color = color(y);
                fix_node = right(y); // could be nil
                if (parent(y) == z)
                {
                    parent(fix_node) = y;
                }
                else
                {
                    MoveSubtree(fix_node, y);
                    right(y) = right(z);
                    parent(right(y)) = y;
                }
                MoveSubtree(y, z);
                left(y) = left(z);
                parent(left(y)) = y;
                color(y) = color(z);

            }

            if (original_color == BLACK)
            {
                FixupDeletion(fix_node);
            }

            return iterator(where.ptr_, this);
        }

        void MoveSubtree(Nodeptr from, Nodeptr to)
        {
            if (root() == to)
            {
                root() = from;
            }
            else if (left(parent(to)) == to)
            {
                left(parent(to)) = from;
            }
            else
            {
                right(parent(to)) = from;
            }
            parent(from) = parent(to);
        }

        void FixupDeletion(Nodeptr x)
        {
            while (x != root() && color(x) == BLACK)
            {
                if (x == left(parent(x)))
                {
                    Nodeptr s = right(parent(x));

                    if (color(s) == RED)
                    {
                        color(s) = BLACK;
                        color(parent(x)) = RED;
                        LeftRotate(parent(x));
                        s = right(parent(x));
                    }

                    // color(s) == BLACK
                    if (color(right(s)) == BLACK && color(left(s)) == BLACK)
                    {
                        color(s) = RED;
                        x = parent(x);
                    }
                    else
                    {
                        if (color(right(s)) == BLACK)  // make it RED
                        {
                            color(s) = RED;
                            color(left(s)) = BLACK;
                            RigitRotate(s);
                            s = right(parent(x));
                        }
                        color(s) = color(parent(x));
                        color(parent(x)) = BLACK;
                        color(right(s)) = BLACK;
                        LeftRotate(parent(x));
                        x = root();
                    }
                } 
                else
                { // Ugly copy paste, just swapping left and right
                    Nodeptr s = left(parent(x));

                    if (color(s) == RED)
                    {
                        color(s) = BLACK;
                        color(parent(x)) = RED;
                        RightRotate(parent(x));
                        s = left(parent(x));
                    }

                    // color(s) == BLACK
                    if (color(left(s)) == BLACK && color(right(s)) == BLACK)
                    {
                        color(s) = RED;
                        x = parent(x);
                    }
                    else
                    {
                        if (color(left(s)) == BLACK)  // make it RED
                        {
                            color(s) = RED;
                            color(right(s)) = BLACK;
                            LeftRotate(s);
                            s = left(parent(x));
                        }
                        color(s) = color(parent(x));
                        color(parent(x)) = BLACK;
                        color(left(s)) = BLACK;
                        RightRotate(parent(x));
                        x = root();
                    }
                }
            }
            color(s) = BLACK;
        }

		Nodeptr& leftmost() const { return left(head_); }
		Nodeptr& rightmost() const { return right(head_); }
		Nodeptr& root() const { return parent(head_); }

		bool Comp(key_type a, key_type b) const { return compare_(a, b); }

	private:
		key_compare compare_;
	};
}
}
