#pragma once
#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <iomanip>

namespace RRND {
  class Basic {
  public:
    /**
     * @brief Set seed to the random engine
     * @param t_seed 32-bit number as random seed
     * @return true When setting successfully
     * @return false When setting failed
     */
    static bool     set_seed(uint32_t t_seed);

    /**
     * @brief Get seed of random engine
     * @return Assigned seed or 0
     */
    static uint32_t get_seed();
        
    /**
     * @brief Random a number of type T(default is uint32_t)
     * @param t_min Minimum number
     * @param t_max Maximum number
     * @return A random number in a range of [min:max]
     */
    template<typename T = uint32_t>
    static T        random(T t_min, T t_max);
  private:

    ///===================================================///
    /// !!IMPORTANT!!
    /// This class could not be used by client
    ///===================================================///
    class Ratio {
    private:
      class CRatioFeature;
      static CRatioFeature* get_ratio_modified_engine();
      static void           add(uint32_t, uint32_t, CRatioFeature*);
      static bool           remove(uint32_t, CRatioFeature*);
      static uint32_t       random(CRatioFeature*);
      static void           dump(CRatioFeature*);
      static void           change_weight(uint32_t, uint32_t, CRatioFeature*);

      template<class T>
      friend class Core;
    };
    ///===================================================///
    template<class T>
    friend class Core;
  };

  template<class CLASS_C = uint32_t>
  class Core {
  public:
    Core();
    /**
     * @brief Add an object to the list of randomize-able objects
     * @param t_obj Object to add
     * @param t_weight Object's weight/total
     */
    void                                      add(CLASS_C const& t_obj, uint32_t t_weight = 10);

    /**
     * @brief Remove object from randomize-able list
     * This function will not remove the object from the list but remove its
     * weight to maintain objects order
     * @param t_position Position to remove
     * @return true When remove successfully
     * @return false When no object was removed
     */
    bool                                      remove(size_t t_position);

    /**
     * @brief Random an object from preset list
     * @return A random object
     */
    CLASS_C const&                            random();

    /**
     * @brief Get the size of the object list
     * @return Number of object in the randomize-able list
     */
    uint32_t                                  size() const;

    /**
     * @brief Get reference to object at a specific position
     * @param t_position Position to get
     * @return Reference of the corresponding object
     */
    CLASS_C&                                  at(size_t t_position);

    /**
     * @brief Dump the object list with object's order and its weight
     */
    void                                      dump() const;

    /**
     * @brief Dump the object list with object's order and its weight with specific function
     * @param f Function to dump object
     */
    template<class FUNCTION>
    void                                      dump(FUNCTION f, std::ostream& t_os = std::cout) const;

    /**
     * @brief Get an object and its weight at a specific position
     * @param t_position Position to get
     * @return A constant reference pair to the object and its own weight
     */
    std::pair<CLASS_C const&, uint32_t> const& at(size_t t_position) const;

    /**
     * @brief Modify available object's weight
     * @param t_pos Position to modify
     * @param t_new_weight New weight
     */
    void                                       change_weight(size_t t_pos, uint32_t t_new_weight);
    
  private:
    // This member indicates maximum obj size, also used as auto-incremented ID
    uint32_t                                  m_idx;
    // This member store: obj, weight
    std::vector<std::pair<CLASS_C, uint32_t>> m_objLst;
    Basic::Ratio::CRatioFeature*              m_randomPtr;
  };
}


// TODO: Change ratio of available objects => DONE



template <class CLASS_C>
RRND::Core<CLASS_C>::Core() :m_idx(0), m_randomPtr(Basic::Ratio::get_ratio_modified_engine()) {
}

template <class CLASS_C>
void RRND::Core<CLASS_C>::add(CLASS_C const& t_obj, uint32_t t_weight) {
  Basic::Ratio::add(static_cast<uint32_t>(m_objLst.size()), t_weight, m_randomPtr);
  m_objLst.emplace_back(std::make_pair(t_obj, t_weight));
}

template <class CLASS_C>
bool RRND::Core<CLASS_C>::remove(size_t t_position) {
  if(Basic::Ratio::remove(static_cast<uint32_t>(t_position), m_randomPtr) == false) {
    return false;
  }

  m_objLst.erase(m_objLst.begin() + t_position);
  return true;
}

template <class CLASS_C>
CLASS_C const& RRND::Core<CLASS_C>::random() {
  return m_objLst.at(Basic::Ratio::random(m_randomPtr)).first;
}

template <class CLASS_C>
uint32_t RRND::Core<CLASS_C>::size() const {
  return m_objLst.size();
}

template <class CLASS_C>
CLASS_C& RRND::Core<CLASS_C>::at(size_t t_position) {
  return m_objLst.at(t_position).first;
}

template <class CLASS_C>
std::pair<CLASS_C const&, uint32_t> const& RRND::Core<CLASS_C>::at(size_t t_position) const {
  return m_objLst.at(t_position);
}

template <class CLASS_C>
void RRND::Core<CLASS_C>::dump() const {
  Basic::Ratio::dump(m_randomPtr);
}

template <class CLASS_C>
template <class FUNCTION>
void RRND::Core<CLASS_C>::dump(FUNCTION f, std::ostream& t_os) const {
  for(uint32_t i = 0; i < m_objLst.size(); ++i) {
    auto const& p = m_objLst[i];
    t_os << "IDX[" << std::setw(10) << std::setfill(' ') << std::right << i << "] - value: " << f(p.first) << ", W: " << p.second << '\n';
  }
}

template <class CLASS_C>
void RRND::Core<CLASS_C>::change_weight(size_t t_pos, uint32_t t_new_weight) {
  Basic::Ratio::change_weight(t_pos, t_new_weight, m_randomPtr);
  m_objLst.at(t_pos).second = t_new_weight;
}
