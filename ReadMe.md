#Priority_Queue

yet another user-implemented priority queue container.

underlying data structure used is heap, but not spefified whether max-heap or min-heap, that is at your discretion by providing your own compare criterion.

----

    template<
        typename T,
        typename compare = std::less<T>,
        typename IncPrio = Inc<T>,
        typename DecPrio = Dec<T>>
    class PriorityQueue;

`compare` defines how to organize the internal heap, class you provide must have its overloaded *operator()* met the following signature:

> bool operator()( const T& lhs, const T& rhs ) const;<br>

> return **true** if interal data structure **need tweak**, otherwise return false.

`IncPrio` and `DecPrio` define how to increase and decrease the priority of elements. Classes you provide must have their respective overloaded *operator()* met the following signature and behavior:

>void operator ()(T& priority, size_t det)<br>
>priority +=/-= det

this header file also have provided two default `IncPrio` and `DecPrio` if they can meet your requirements.

###Member Types

<table>
<tr>
	<td><strong>Type</strong></td><td><strong>Definition</strong></td>
</tr>
<tr>
	<td>value_type</td><td>T</td>
</tr>
<tr>
	<td>size_type</td><td>size_t</td>
</tr>
<tr>
	<td>pos_iter</td><td>size_t</td>
</tr>
</table>


###Public Member Interfaces
<table>
   <tr>
      <td>PriorityQueue</td>
      <td>Construct a priority queue that is empty or is a copy of a range of array or is a copy of other priority queue</td>
   </tr>
   <tr>
      <td>operator =</td>
      <td>operator assignement</td>
   </tr>
   <tr>
      <td>top</td>
      <td>return the element that has highest priority</td>
   </tr>
   <tr>
      <td>ExtractTop</td>
      <td>remove the element that has highest priority from the queue</td>
   </tr>
   <tr>
      <td>insert</td>
      <td>add an element into the queue</td>
   </tr>
   <tr>
      <td>find</td>
      <td>find and return the position of the specified element</td>
   </tr>
   <tr>
      <td>IncreasePriority</td>
      <td>increase priority of a specified element. e.g to make it more closer to the root.</td>
   </tr>
   <tr>
      <td>DecreasePriority</td>
      <td>decrease priority of a specified element. Contrary to IncreasePriority</td>
   </tr>
   <tr>
      <td>Delete</td>
      <td>remove a specified element from the queue</td>
   </tr>
   <tr>
      <td>swap</td>
      <td>swap two priority queues</td>
   </tr>
   <tr>
      <td>size</td>
      <td>return count of elements in the queue</td>
   </tr>
   <tr>
      <td>capacity</td>
      <td>return size of internal heap</td>
   </tr>
   <tr>
      <td>empty</td>
      <td>check whether the queue is empty</td>
   </tr>
</table>

###Public Non-member Interface
<table>
   <tr>
      <td>swap</td>
      <td>non-member version of swap</td>
   </tr>
</table>