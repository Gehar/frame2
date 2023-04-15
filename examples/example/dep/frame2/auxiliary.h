/*
 * Auxiliary.h
 *
 *  Created on: 13 авг. 2019 г.
 *      Author: user
 */

#ifndef FRAME2_AUXILIARY_H_
#define FRAME2_AUXILIARY_H_

#include <cstdint>

#include <frame2/buffer.h>
#include <frame2/const_buffer.h>

namespace frame2
{

  class Auxiliary
  {
  public:
    Auxiliary ();
    ~Auxiliary ();

    static const uint8_t scValueUndefined = 0xff;

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

  private:

    struct Field
    {
      enum c
      {
        AMPLITUDE_DATA = 0, AMPLITUDE_PILOT, PHASE_STD_DATA, PHASE_STD_PILOT, DATA_NUM, PILOT_NUM,
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

    static const Binary scDefaultBinary;
    static const float NaN;

    constexpr inline uint8_t Flag (Field::c field) const
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

} /* namespace frame2 */

#endif /* FRAME2_AUXILIARY_H_ */
