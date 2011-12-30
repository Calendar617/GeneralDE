#ifndef CPEPP_UTILS_OPGUARD_H
#define CPEPP_UTILS_OPGUARD_H
#include <vector>
#include <memory>
#include "ClassCategory.hpp"

namespace Cpe { namespace Utils {

class OpGuard : public Noncopyable {
public:
    class OpGuardNode {
    public:
        virtual void execute(void) = 0;
        virtual ~OpGuardNode() = 0;
    };

    OpGuard();
    ~OpGuard();

    template<typename T>
    class OpGuardNode_MemFun : public OpGuardNode {
    public:
        OpGuardNode_MemFun(T & o, void (T::*fun)(void))
            : m_o(o)
            , m_fun(fun)
        {
        }

        virtual void execute(void) {
            (m_o.*m_fun)();
        }

        T & m_o;
        void (T::*m_fun)(void);
    };

    template<typename T>
    void addOp(T & o, void (T::*fun)(void)) {
        this->addOpNode(
            ::std::auto_ptr<OpGuardNode>(
                new OpGuardNode_MemFun<T>(o, fun)));
    }

    void releaseControl(void);

private:
    void addOpNode(::std::auto_ptr<OpGuardNode> node);

    bool m_needExecute;
    ::std::vector<OpGuardNode*> m_ops;
};

}}

#endif
