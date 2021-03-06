#pragma once

#include <memory>

#include "mcga/test.hpp"

namespace mcga::test {

/** Internal representation of a group.
 *
 * This class should only be instantiated once per call to the group() function
 * from the public API. The current convention is that every instance is created
 * by the testing Driver, but is created as a std::shared_ptr, to avoid
 * premature deletion (extensions currently have direct access to the pointer,
 * and could have invalid access if the Driver decides to delete the instance).
 *
 * Same as Test, this is an _upgrade_ to the GroupConfig received by the group()
 * function, and therefore inherits privately from GroupConfig (same as Test,
 * config items are immutable once received into the system). Unlike Test,
 * however, a Group is not _immutable_ (e.g. addSetUp(), addTearDown() and
 * others). */
class Group : private GroupConfig {
  public:
    /** The pointer type that should be passed around when working with a
     * Group. */
    using Ptr = std::shared_ptr<Group>;

    /** Default constructor from the upgraded GroupConfig and extra metadata
     * received from the testing Driver. Very similar to Test#Test().*/
    Group(GroupConfig config, Ptr parentGroup, int id);

    /** See GroupConfig#description. */
    std::string getDescription() const;

    /** See GroupConfig#optional. */
    bool isOptional() const;

    /** Globally unique, incremental identifier of the group. */
    int getId() const;

    /** Pointer to the Group that contains this, or `nullptr` if the current
     * group represents a TestCase. */
    Ptr getParentGroup() const;

    /** Add a set-up function to this group.
     *
     * This method is called once for every setUp(Executable) call from the
     * global API, on the instance that is currently on top of the group
     * stack.
     *
     * Note: This throws on being called the second time for the same group. */
    void addSetUp(Executable func);

    /** Returns whether this group has a setUp.
     *
     * Is true only if addSetUp() was called on this instance. */
    bool hasSetUp() const;

    /** Call the set-up function of this group. */
    void setUp() const;

    /** Add a tear-down function to this group.
     *
     * This method is called once for every tearDown(Executable) call from the
     * global API, on the instance that is currently on top of the group
     * stack.
     *
     * Note: This throws on being called the second time for the same group. */
    void addTearDown(Executable func);

    /** Returns whether this group has a tearDown.
     *
     * Is true only if addTearDown() was called on this instance. */
    bool hasTearDown() const;

    /** Call the tear-down function of this group. */
    void tearDown() const;

  private:
    Ptr parentGroup;
    int id;

    Executable setUpFunc;
    Executable tearDownFunc;
};

using GroupPtr = Group::Ptr;

}  // namespace mcga::test
