#pragma once

#include <list>
#include <memory>

/// <summary>
/// ����� ��� �������� ������ (Undo - Redo)
/// </summary>
/// <typeparam name="T">��� ��� �������� ����� ������������ �������� ������</typeparam>
template<typename T>
class ChangesManager
{
private:
	using val = T*;
	using collection = std::list<T*>;
	collection previusValues, nextValues;
public:
	~ChangesManager();

	/// <summary>
	/// ������������ ����������� ������� ���������
	/// </summary>
	static const int maxSize = 25;
private:
	void RemoveFirst(collection& data);
	val DropLast(collection& data);
	void AddWithLimit(collection& data, val value);
public:

	/// <summary>
	/// ������� ��������� ������� � �����������
	/// </summary>
	/// <param name="valueNow">������� ��������� �������</param>
	/// <returns>���������� ���������� ��������� �������, ���� ��� ��� ���������� valueNow</returns>
	val Undo(val valueNow);

	/// <summary>
	/// �������� ����� ��������� ������� � ����������� ���������
	/// </summary>
	/// <param name="valueNow">������� ��������� �������</param>
	/// <returns>���������� ��������� ������� �� ������, ���� ��� ��� ���������� valueNow</returns>
	val Redo(val valueNow);

	/// <summary>
	/// ������ ������ ��������� �������� ����� � ���� ����� ���� ���������
	/// </summary>
	/// <param name="valueNow">������� ��������� �������</param>
	void AddPrevValue(val valueNow);
private:
	void AddNext(val nextValue);
	void AddPrev(val prevValue);

public:
	/// <summary>
	/// �������� ������� ���������
	/// </summary>
	void Clear();
};

template<typename T>
ChangesManager<T>::~ChangesManager()
{
	for (auto& value : previusValues) if (value) delete value;
	for (auto& value : nextValues) if (value) delete value;
}

template<typename T>
inline void ChangesManager<T>::RemoveFirst(collection& data)
{
	if (!data.empty())
	{
		delete* std::begin(data);
		data.pop_front();
	}
}

template<typename T>
inline auto ChangesManager<T>::DropLast(collection& data) -> val
{
	auto last = *std::prev(std::end(data));
	data.pop_back();
	return last;
}

template<typename T>
inline void ChangesManager<T>::AddWithLimit(collection& data, val value)
{
	data.emplace_back(value);

	while (data.size() >= maxSize)
		RemoveFirst(data);
}

template<typename T>
inline auto ChangesManager<T>::Undo(val valueNow) -> val
{
	if (previusValues.empty()) return valueNow;
	AddNext(valueNow);
	return DropLast(previusValues);
}

template<typename T>
inline auto ChangesManager<T>::Redo(val valueNow) -> val
{
	if (nextValues.empty()) return valueNow;
	AddPrev(valueNow);
	return DropLast(nextValues);
}

template<typename T>
inline void ChangesManager<T>::AddPrevValue(val valueNow)
{
	nextValues.clear();
	AddPrev(valueNow);
}

template<typename T>
inline void ChangesManager<T>::AddNext(val nextValue)
{
	AddWithLimit(nextValues, nextValue);
}

template<typename T>
inline void ChangesManager<T>::AddPrev(val prevValue)
{
	AddWithLimit(previusValues, prevValue);
}

template<typename T>
inline void ChangesManager<T>::Clear()
{
	nextValues.clear();
	previusValues.clear();
}
