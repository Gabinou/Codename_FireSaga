#ifndef GENERIC_HPP
#define GENERIC_HPP
/*! \file generic.hpp
* \brief Generic class that as many classes as possible inherit from.
*/

/// \class generic
/// \brief Class from which the most objects inherit from.
class generic {
public:
    char name[120];
    /// \var char name
    /// \brief Name of generic object.
    unsigned char id;
    /// \var unsigned char id
    /// \brief All objects get ids.
    char type[24]; 
    /// \var type
    /// \brief For units, contains class string. For weapons, contains weapon type string.
    ~generic();
    generic();
    generic(const generic &obj);
    /// \fn generic(const generic &obj)
    /// \brief Copy constructor for generic object. Useless?
};


#endif /* GENERIC_HPP */