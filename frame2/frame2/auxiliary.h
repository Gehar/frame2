/*
 * Auxiliary.h
 *
 *  Created on: 13 авг. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#ifndef FRAME2_AUXILIARY_H_
#define FRAME2_AUXILIARY_H_

#include <cstdint>

#include <frame2/buffer.h>
#include <frame2/const_buffer.h>
#include <frame2/array.h>

#if  (__GNUC__ < 7) || \
    ((__GNUC__ == 7) && (__GNUC_MINOR__ < 2))

// gcc версии ниже 7.2.0 не поддерживает "constexpr inline" методы
#define CONSTEXPR_INLINE

#else

// constexpr inline методы поддерживаются gcc, начиная с версии 7.2.0
#define CONSTEXPR_INLINE constexpr inline

#endif

namespace frame2
{

  class Auxiliary
  {
  public:
    Auxiliary ();
    ~Auxiliary ();

    static const uint8_t scValueUndefined = 0xff;
    static constexpr size_t scNumAntennas = 8;
    static_assert (scNumAntennas < 128, "scNumAntennas can not be more 127. In order to increase limit contact with developer");
    typedef frame2::Array<float, scNumAntennas> ArrayFloat;

    inline void Clear();
    inline size_t ByteSize() const;

    inline size_t SerializeToArray(void* data) const;
    inline size_t ParseFromArray(const void* data);

    // accessors -------------------------------------------------------

    // state - текущее состояние канала.
    inline int8_t state() const;
    inline void set_state(int8_t value);

    // flags - флаги состояния канала:
    //   Бит 0 - принимаемый сигнал прошел процедуру валидности, измерения могут быть использованы для дальнейшей обработки.
    //   Назначение остальных битов может зависить от модели прибора и версии прошивки.
    inline uint32_t flags() const;
    inline void set_flags(uint32_t value);

    // ampl_data - опциональное поле. Амплитуда информационной компоненты (в том числе ГЛОНАСС L1OF, L2OF)
    inline bool has_ampl_data() const;
    inline void clear_ampl_data();
    inline float ampl_data() const; // если поле отсутствует, вернет NaN
    inline void set_ampl_data(float value); // если отсутствует, то оно будет добавлено

    // ampl_pilot - опциональное поле. Амплитуда информационной компоненты (в том числе ГЛОНАСС L1OF, L2OF)
    inline bool has_ampl_pilot() const;
    inline void clear_ampl_pilot();
    inline float ampl_pilot() const; // если поле отсутствует, вернет NaN
    inline void set_ampl_pilot(float value); // если отсутствует, то оно будет добавлено

    // phase_std_data - опциональное поле. СКО ошибки информационной компоненты на выходе фазового дискриминатора (в том числе для L1OF, L2OF)
    inline bool has_phase_std_data() const;
    inline void clear_phase_std_data();
    inline float phase_std_data() const; // если поле отсутствует, вернет NaN
    inline void set_phase_std_data(float value); // если отсутствует, то оно будет добавлено

    // phase_std_pilot - опциональное поле. СКО ошибки информационной компоненты на выходе фазового дискриминатора (в том числе для L1OF, L2OF)
    inline bool has_phase_std_pilot() const;
    inline void clear_phase_std_pilot();
    inline float phase_std_pilot() const; // если поле отсутствует, вернет NaN
    inline void set_phase_std_pilot(float value); // если отсутствует, то оно будет добавлено

    // data_num - номер информационной компоненты для двухкомпонентных сигналов
    inline bool has_data_num() const;
    inline void clear_data_num();
    inline uint8_t data_num() const; // если поле отсутствует, вернет scValueUndefined
    inline void set_data_num(uint8_t value); // если отсутствует, то оно будет добавлено

    // pilot_num - номер пилотной компоненты для двухкомпонентных сигналов
    inline bool has_pilot_num() const;
    inline void clear_pilot_num();
    inline uint8_t pilot_num() const; // если поле отсутствует, вернет scValueUndefined
    inline void set_pilot_num(uint8_t value); // если отсутствует, то оно будет добавлено

    // dif_phase_ants - массив с разностями фаз между антеннами
    inline bool has_dif_phase_ants() const;
    inline void clear_dif_phase_ants();
    inline const ArrayFloat& dif_phase_ants() const; // если поле отсутствует, вернет scValueUndefined
    // Возвращаем ссылку на ArrayFloat, т.к. для указателя ArrayFloat* вместо оператора frame2::Array::operator[] будет вызываться operator[] для массива из ArrayFloat
    inline ArrayFloat& add_dif_phase_ants(); // Поднимает флаг наличия поля и возвращает указатель изменяемый объект
    // Возвращаем ссылку на ArrayFloat, т.к. для указателя ArrayFloat* вместо оператора frame2::Array::operator[] будет вызываться operator[] для массива из ArrayFloat
    inline ArrayFloat& mutable_dif_phase_ants(); // Возвращает указатель на объект для изменения. Если объект не был ранее добавлен, используйте метод add_dif_phase_ants()
    inline void set_dif_phase_ants(const ArrayFloat &array); // если отсутствует, то оно будет добавлено

  private:

    struct Field
    {
      enum c
      {
        AMPLITUDE_DATA = 0, AMPLITUDE_PILOT, PHASE_STD_DATA, PHASE_STD_PILOT, DATA_NUM, PILOT_NUM, DPHASE_ANT,
      };
    };

    /* Постоянная часть бинарной структуры */
#pragma pack(push,1)
    struct Binary
    {
      int8_t  state;
      uint32_t flags;
      uint8_t fields_existence;
    };
#pragma pack(pop)

    Binary mBinary;

    float mAmplData;
    float mAmplPilot;
    float mPhaseStdData;
    float mPhaseStdPilot;
    uint8_t mDataNum;
    uint8_t mPilotNum;
    /* Разность фаз между антеннами */
    ArrayFloat mDifPhaseAnt;

    static const Binary scDefaultBinary;
    static const float NaN;

    CONSTEXPR_INLINE uint8_t Flag (Field::c field) const
    {
      return static_cast<uint8_t>(1 << field);
    }
  };

  inline void
  Auxiliary::Clear()
  {
    mBinary = scDefaultBinary;
    mAmplData = NaN;
    mAmplPilot = NaN;
    mPhaseStdData = NaN;
    mPhaseStdPilot = NaN;
    mDataNum = scValueUndefined;
    mPilotNum = scValueUndefined;
    mDifPhaseAnt.resize(0);
  }

  inline size_t
  Auxiliary::ByteSize() const
  {
    size_t size = 0;
    size += sizeof(Binary);
    if (has_ampl_data())
    {
      size += sizeof(mAmplData);
    }
    if (has_ampl_pilot())
    {
      size += sizeof(mAmplPilot);
    }
    if (has_phase_std_data())
    {
      size += sizeof(mPhaseStdData);
    }
    if (has_phase_std_pilot())
    {
      size += sizeof(mPhaseStdPilot);
    }
    if (has_data_num())
    {
      size += sizeof(mDataNum);
    }
    if (has_pilot_num())
    {
      size += sizeof(mPilotNum);
    }
    if (has_dif_phase_ants())
    {
      size += sizeof(uint8_t);
      size += mDifPhaseAnt.size() * sizeof(mDifPhaseAnt[0]);
    }
    return size;
  }

  inline size_t
  Auxiliary::SerializeToArray(void* data) const
  {
    size_t size = ByteSize ();
    Buffer buf(data, size);
    buf.Add(mBinary);
    if (has_ampl_data())
      buf.Add(mAmplData);
    if (has_ampl_pilot())
      buf.Add(mAmplPilot);
    if (has_phase_std_data())
      buf.Add(mPhaseStdData);
    if (has_phase_std_pilot())
      buf.Add(mPhaseStdPilot);
    if (has_data_num())
      buf.Add(mDataNum);
    if (has_pilot_num())
      buf.Add(mPilotNum);
    if (has_dif_phase_ants())
    {
      buf.Add(static_cast<uint8_t>(mDifPhaseAnt.size()));
      for (size_t i = 0; i < mDifPhaseAnt.size(); i++)
        buf.Add(mDifPhaseAnt[i]);
    }
    return buf.Position();
  }

  inline size_t
  Auxiliary::ParseFromArray(const void* data)
  {
    size_t max_size = sizeof(mBinary) + sizeof(mAmplData) + sizeof(mPhaseStdData);
    ConstBuffer buf(data, max_size);
    buf.Get(mBinary);
    if (has_ampl_data())
      buf.Get(mAmplData);
    if (has_ampl_pilot())
      buf.Get(mAmplPilot);
    if (has_phase_std_data())
      buf.Get(mPhaseStdData);
    if (has_phase_std_pilot())
      buf.Get(mPhaseStdPilot);
    if (has_data_num())
      buf.Get(mDataNum);
    if (has_pilot_num())
      buf.Get(mPilotNum);
    if (has_dif_phase_ants())
    {
      uint8_t size = 0;
      buf.Get(size);
      mDifPhaseAnt.resize(size);
      for (size_t i = 0; i < mDifPhaseAnt.size(); i++)
      {
        buf.Get(mDifPhaseAnt[i]);
      }

      // Если данных больше, чем выделено памяти, то дочитываем их
      if (mDifPhaseAnt.size() < size)
      {
        float tmp = 0;
        for (size_t i = mDifPhaseAnt.size(); i < size; i++)
        {
          buf.Get(tmp);
        }
      }
    }
    return buf.Position();
  }

  // accessors -------------------------------------------------------

  // channel
  inline int8_t
  Auxiliary::state() const
  {
    return mBinary.state;
  }

  inline void
  Auxiliary::set_state(int8_t value)
  {
    mBinary.state = value;
  }

  // flags - флаги состояния канала
  inline uint32_t
  Auxiliary::flags() const
  {
    return mBinary.flags;
  }
  inline void
  Auxiliary::set_flags(uint32_t value)
  {
    mBinary.flags = value;
  }

  // ampl_data - опциональное поле. амплитуда информационной компоненты (в том числе для L1OF, L2OF)
  inline bool
  Auxiliary::has_ampl_data() const
  {
    return (mBinary.fields_existence & Flag(Field::AMPLITUDE_DATA)) != 0;
  }
  inline void
  Auxiliary::clear_ampl_data()
  {
    mBinary.fields_existence = mBinary.fields_existence & static_cast<uint8_t>(~Flag(Field::AMPLITUDE_DATA));
    mAmplData = NaN;
  }
  inline float
  Auxiliary::ampl_data() const // если поле отсутствует, вернет NaN
  {
    return mAmplData;
  }
  inline void
  Auxiliary::set_ampl_data(float value) // если отсутствует, то оно будет добавлено
  {
    mBinary.fields_existence = mBinary.fields_existence | Flag(Field::AMPLITUDE_DATA);
    mAmplData = value;
  }

  // ampl_pilot - опциональное поле. амплитуда пилотной компоненты
  inline bool
  Auxiliary::has_ampl_pilot() const
  {
    return (mBinary.fields_existence & Flag(Field::AMPLITUDE_PILOT)) != 0;
  }
  inline void
  Auxiliary::clear_ampl_pilot()
  {
    mBinary.fields_existence = mBinary.fields_existence & static_cast<uint8_t>(~Flag(Field::AMPLITUDE_PILOT));
    mAmplPilot = NaN;
  }
  inline float
  Auxiliary::ampl_pilot() const // если поле отсутствует, вернет NaN
  {
    return mAmplPilot;
  }
  inline void
  Auxiliary::set_ampl_pilot(float value) // если отсутствует, то оно будет добавлено
  {
    mBinary.fields_existence = mBinary.fields_existence | Flag(Field::AMPLITUDE_PILOT);
    mAmplPilot = value;
  }

  // phase_std_data - опциональное поле. СКО ошибки информационной компоненты на выходе фазового дискриминатора (в том числе для L1OF, L2OF)
  inline bool
  Auxiliary::has_phase_std_data() const
  {
    return (mBinary.fields_existence & Flag(Field::PHASE_STD_DATA)) != 0;
  }
  inline void
  Auxiliary::clear_phase_std_data()
  {
    mBinary.fields_existence = mBinary.fields_existence & static_cast<uint8_t>(~Flag(Field::PHASE_STD_DATA));
    mPhaseStdData = NaN;
  }
  inline float
  Auxiliary::phase_std_data() const // если поле отсутствует, вернет NaN
  {
    return mPhaseStdData;
  }
  inline void
  Auxiliary::set_phase_std_data(float value) // если отсутствует, то оно будет добавлено
  {
    mBinary.fields_existence = mBinary.fields_existence | Flag(Field::PHASE_STD_DATA);
    mPhaseStdData = value;
  }

  // phase_std_pilot - опциональное поле. СКО ошибки пилотной компоненты на выходе фазового дискриминатора
  inline bool
  Auxiliary::has_phase_std_pilot() const
  {
    return (mBinary.fields_existence & Flag(Field::PHASE_STD_PILOT)) != 0;
  }
  inline void
  Auxiliary::clear_phase_std_pilot()
  {
    mBinary.fields_existence = mBinary.fields_existence & static_cast<uint8_t>(~Flag(Field::PHASE_STD_PILOT));
    mPhaseStdPilot = NaN;
  }
  inline float
  Auxiliary::phase_std_pilot() const // если поле отсутствует, вернет NaN
  {
    return mPhaseStdPilot;
  }
  inline void
  Auxiliary::set_phase_std_pilot(float value) // если отсутствует, то оно будет добавлено
  {
    mBinary.fields_existence = mBinary.fields_existence | Flag(Field::PHASE_STD_PILOT);
    mPhaseStdPilot = value;
  }

  // data_num - номер информационной компоненты для двухкомпонентных сигналов
  inline bool
  Auxiliary::has_data_num() const
  {
    return (mBinary.fields_existence & Flag(Field::DATA_NUM)) != 0;
  }
  inline void
  Auxiliary::clear_data_num()
  {
    mBinary.fields_existence = mBinary.fields_existence & static_cast<uint8_t>(~Flag(Field::DATA_NUM));
    mDataNum = scValueUndefined;
  }
  inline uint8_t
  Auxiliary::data_num() const // если поле отсутствует, вернет scValueUndefined
  {
    return mDataNum;
  }
  inline void
  Auxiliary::set_data_num(uint8_t value) // если отсутствует, то оно будет добавлено
  {
    mBinary.fields_existence = mBinary.fields_existence | Flag(Field::DATA_NUM);
    mDataNum = value;
  }

  // pilot_num - номер пилотной компоненты для двухкомпонентных сигналов
  inline bool
  Auxiliary::has_pilot_num() const
  {
    return (mBinary.fields_existence & Flag(Field::PILOT_NUM)) != 0;
  }
  inline void
  Auxiliary::clear_pilot_num()
  {
    mBinary.fields_existence = mBinary.fields_existence & static_cast<uint8_t>(~Flag(Field::PILOT_NUM));
    mPilotNum = scValueUndefined;
  }
  inline uint8_t
  Auxiliary::pilot_num() const // если поле отсутствует, вернет scValueUndefined
  {
    return mPilotNum;
  }
  inline void
  Auxiliary::set_pilot_num(uint8_t value) // если отсутствует, то оно будет добавлено
  {
    mBinary.fields_existence = mBinary.fields_existence | Flag(Field::PILOT_NUM);
    mPilotNum = value;
  }

  // dif_phase_ants - номер пилотной компоненты для двухкомпонентных сигналов
  inline bool
  Auxiliary::has_dif_phase_ants() const
  {
    return (mBinary.fields_existence & Flag(Field::DPHASE_ANT)) != 0;
  }
  inline void
  Auxiliary::clear_dif_phase_ants()
  {
    mBinary.fields_existence = mBinary.fields_existence & static_cast<uint8_t>(~Flag(Field::DPHASE_ANT));
    mDifPhaseAnt.resize(0);
  }
  inline const Auxiliary::ArrayFloat&
  Auxiliary::dif_phase_ants() const // если поле отсутствует, вернет scValueUndefined
  {
    return mDifPhaseAnt;
  }
  inline Auxiliary::ArrayFloat&
  Auxiliary::add_dif_phase_ants()
  {
    mBinary.fields_existence = mBinary.fields_existence | Flag(Field::DPHASE_ANT);
    return mDifPhaseAnt;
  }
  inline Auxiliary::ArrayFloat&
  Auxiliary::mutable_dif_phase_ants()
  {
    return mDifPhaseAnt;
  }
  inline void
  Auxiliary::set_dif_phase_ants(const Auxiliary::ArrayFloat &object) // если отсутствует, то оно будет добавлено
  {
    mBinary.fields_existence = mBinary.fields_existence | Flag(Field::DPHASE_ANT);
    mDifPhaseAnt = object;
  }

} /* namespace frame2 */

#endif /* FRAME2_AUXILIARY_H_ */
