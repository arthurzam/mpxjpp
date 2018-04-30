#ifndef ENTRY_H
#define ENTRY_H

#include <string>

namespace poifs {
namespace filesystem {

class DirectoryEntry;

/**
 * This interface provides access to an object managed by a Filesystem
 * instance. Entry objects are further divided into DocumentEntry and
 * DirectoryEntry instances.
 */
class Entry {
protected:
    DirectoryEntry *m_parent;
public:
    virtual ~Entry() = default;

    /// get the name of the Entry
    virtual std::string name() const = 0;

    /// is this a DirectoryEntry?
    virtual bool isDirectoryEntry() const = 0;

    /// is this a DocumentEntry?
    virtual bool isDocumentEntry() const = 0;

    /// get this Entry's parent (the DirectoryEntry that owns this Entry).
    /// All Entry objects, except the root Entry, has a parent.
    DirectoryEntry *parent() const {
        return m_parent;
    }

    /**
     * Delete this Entry. This operation should succeed, but there are
     * special circumstances when it will not:
     *
     * If this Entry is the root of the Entry tree, it cannot be
     * deleted, as there is no way to create another one.
     *
     * If this Entry is a directory, it cannot be deleted unless it is
     * empty.
     *
     * @return true if the Entry was successfully deleted, else false
     */
    virtual bool remove() = 0;

    /**
     * Rename this Entry. This operation will fail if:
     *
     * There is a sibling Entry (i.e., an Entry whose parent is the
     * same as this Entry's parent) with the same name.
     *
     * This Entry is the root of the Entry tree. Its name is dictated
     * by the Filesystem and many not be changed.
     *
     * @param newName the new name for this Entry
     *
     * @return true if the operation succeeded, else false
     */
    virtual bool renameTo(const std::string &newName) = 0;
};

/**
 * This interface defines methods specific to Document objects
 * managed by a Filesystem instance.
 */
class DocumentEntry : public Entry {
public:
    /**
     * get the zize of the document, in bytes
     *
     * @return size in bytes
     */
    virtual size_t size() const = 0;
};

class DirectoryEntry : public Entry {
public:
    // Iterator<Entry> getEntries();
};

}
}

#endif // ENTRY_H
