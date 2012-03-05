topic "ValueArray";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 ValueArray]]}}&]
[s1;@(0.0.255)3 &]
[s1;:ValueArray`:`:class: [@(0.0.255)3 class][3 _][*3 ValueArray][3 _]&]
[s2;%% ValueArray is an array of Value elements. It is fully Rich 
Value compatible.&]
[s2;%% &]
[s2;%% ValueArray is also compatible with ValueMap (in the similar 
way how Date is compatible with Time). Assigning Value containing 
ValueMap to ValueArray assigns it a ValueArray representing values 
of ValueMap. Assigning ValueArray to ValueMap creates map where 
keys are number 0, 1, ... GetCount() `- 1 and values contain 
elements with corresponding indices.&]
[s2;%% &]
[s2;%% Not that Value also provides methods (GetCount() and operator`[`]) 
to directly access ValueArray elements contained in Value (if 
Value does not contain ValueArray, GetCount() returns 0).&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:ValueArray`:`:ValueArray`(`): [* ValueArray]()&]
[s2;%% Creates an empty ValueArray.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(const ValueArray`&`): [* ValueArray]([@(0.0.255) const]_[* V
alueArray][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Copy constructor.&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(pick`_ Vector`<Value`>`&`): [@(0.0.255) explicit]_[* Value
Array]([@(0.128.128) pick`_]_[_^Vector^ Vector]<[_^Value^ Value]>`&_[*@3 values])&]
[s2;%% Creates ValueArray by [%-*@3 values].&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(const Vector`<Value`>`&`,int`): [@(0.0.255) explicit]_[* V
alueArray]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Value^ Value]>`&_[*@3 values], 
[@(0.0.255) int]_[*@3 deep])&]
[s2;%% Creates ValueArray as deep copy of [%-*@3 values] ([%-*@3 deep] 
is dummy parameter to make the signature different from picking 
version).&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:`~ValueArray`(`): [@(0.0.255) `~][* ValueArray]()&]
[s2;%% Destructor.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:operator`=`(const ValueArray`&`): [_^ValueArray^ ValueArray][@(0.0.255) `&
]_[* operator`=]([@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Assignment operator.&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:operator Value`(`)const: [* operator_Value]()_[@(0.0.255) const]&]
[s2;%% Returns ValueArray as Value.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(const Value`&`): [* ValueArray]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 src])&]
[s2;%% Gets ValueArray from Value [%-*@3 src]. Note that this also 
works if Value contains ValueMap (vector of values, of key`-value 
pairs, is returned).&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(const Nuller`&`): [* ValueArray]([@(0.0.255) const]_[_^Nuller^ N
uller][@(0.0.255) `&])&]
[s2;%% Creates empty ValueArray (Null assignment).&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:IsNull`(`)const: [@(0.0.255) bool]_[* IsNull]()_[@(0.0.255) const]&]
[s2;%% Returns true if there are no elements.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Removes all elements.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:SetCount`(int`): [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 n])&]
[s2;%% Sets the count of elements to [%-*@3 n]. If number of elements 
increases, new elements are void Values.&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:SetCount`(int`,const Value`&`): [@(0.0.255) void]_[* SetCount]([@(0.0.255) i
nt]_[*@3 n], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Sets the count of elements to [%-*@3 n]. If number of elements 
increases, new elements are set to [%-*@3 v] .&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%% Returns number of elements.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:IsEmpty`(`)const: [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2;%% Same as GetCount() `=`= 0 (or as IsNull()).&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Add`(const Value`&`): [@(0.0.255) void]_[* Add]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Adds new element [%-*@3 v].&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:operator`<`<`(const Value`&`): [_^ValueArray^ ValueArray][@(0.0.255) `&
]_[* operator<<]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Same as Add([%-*@3 v]).&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:Set`(int`,const Value`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Sets element at [%-*@3 i] to [%-*@3 v]. If [%-*@3 i] >`= GetCount(), 
void Values are added so that element [%-*@3 i] created.&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:Get`(int`)const: [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* Get
]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns element at [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:GetAndClear`(int`): [_^Value^ Value]_[* GetAndClear]([@(0.0.255) int]_[*@3 i
])&]
[s2;%% Assigns empty Value at [%-*@3 i] and returns Value that was 
in this element before. This special method is intended as optimization 
in certain situation, as it can void expensive cloning of Value 
in situation where keeping original Value in ValueArray is not 
required.&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:Get`(`)const: [@(0.0.255) const]_[_^Vector^ Vector]<[_^Value^ Value]>`&_
[* Get]()_[@(0.0.255) const]&]
[s2;%% Returns constant reference to (internal) Vector<Value>.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Remove`(int`,int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2;%% Removes [%-*@3 count] elements at [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:Insert`(int`,const ValueArray`&`): [@(0.0.255) void]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 va])&]
[s2;%% Inserts another ValueArra [%-*@3 va] at [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:Append`(const ValueArray`&`): [@(0.0.255) void]_[* Append]([@(0.0.255) c
onst]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 va])&]
[s2;%% Appends [%-*@3 va] at the end of ValueArray.&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns element at [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:GetHashValue`(`)const: [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2;%% Returns hashing value.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream
][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Serializes ValueArray. All elements must have serialization 
implemented.&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:Jsonize`(JsonIO`&`): [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2;%% Converts ValueArray to/from JSON. All elements must have JSON 
conversion implemented.&]
[s3;%% &]
[s4; &]
[s5;:ValueArray`:`:operator`=`=`(const ValueArray`&`)const: [@(0.0.255) bool]_[* operator
`=`=]([@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) con
st]&]
[s5;:ValueArray`:`:operator`!`=`(const ValueArray`&`)const: [@(0.0.255) bool]_[* operator
!`=]([@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) cons
t]&]
[s2;%% (In)Equality comparison. Two ValueArrays are equal if they 
have the same number of elements and all elements at the same 
position are equal.&]
[s0;%% ]