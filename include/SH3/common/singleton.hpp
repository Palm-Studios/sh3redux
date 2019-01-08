/** @file
 *  Singleton template class. Any object that <i>must</i> have only one instance (such as the game
 *  engine itself, or it's specific managers [i.e scene, texture, shader]) should inherit this class.
 *
 *  @copyright 2016-2019 Palm Studios
 *
 *  @date 8-1-2019
 *
 *  @author Jesse Buhagiar
 */

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace sh3
{

template<typename T>
class CSingleton
{
public:
    typedef T SingletonType; /**< Base type of this singleton */

public:
    /**
     * Copy constructor
     *
     * @note This is deleted. We do NOT want to copy a singleton under any circumstance
     */
    CSingleton(const CSingleton& singleton) = delete;

    /**
     * Move constructor
     *
     * @note This is deleted. We do NOT want to copy a singleton under any circumstance
     */
    CSingleton(const CSingleton&& singleton) = delete;

    /**
     * Assignment operator.
     *
     * @note This is deleted. We do NOT want to copy a singleton under any circumstance
     */
    T& operator=(CSingleton& singleton) = delete;

    /**
     * Move assignment operator
     *
     * @note This is deleted. We do NOT want to copy a singleton under any circumstance
     */
    T& operator=(CSingleton&& singleton) = delete;


    /**
     * Get the instance of this singleton. If no instance exists, create one and then return it.
     *
     * @return Pointer to singleton instance of this object.
     *
     * @note This object is immutable!
     */
    static T& Instance(void)
    {
        /**
         * According to the C++ standard:
         *
         *
         * §6.7 [stmt.dcl] p4 If control enters the declaration concurrently while the variable is being
         * initialized, the concurrent execution shall wait for completion of the initialization.
         *
         * Obtaining the singleton instance in this way will be threadsafe
         */
        static T instance;

        return instance;
    }

protected:

    CSingleton(void)
    {
    }
    virtual ~CSingleton(void){}
};

}

#endif /* INCLUDE_SH3_COMMON_SINGLETON_HPP_ */
