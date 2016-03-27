#pragma once

namespace sdk
{
    struct ContainerBase
    {
        void SwapAll(ContainerBase&)
        {
        }
    };

    struct IteratorBase
    {
        const ContainerBase *GetContainer() const
        {
            return cont_;
        }

		void Adopt(const ContainerBase *cont)
		{
			cont_ = cont;
		}
		const ContainerBase *cont_;
    };
}
