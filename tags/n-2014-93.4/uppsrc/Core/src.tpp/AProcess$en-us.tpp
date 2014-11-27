topic "AProcess and LocalProcess";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 AProcess]]}}&]
[s3; &]
[s1;:AProcess`:`:struct: [@(0.0.255)3 struct][3 _][*3 AProcess][3 _:_][@(0.0.255)3 public][3 _][*@3;3 N
oCopy]&]
[s9;%% This is an abstract class that represent a controlled child 
process. Client code can communicate with such process by supplying 
standard input and reading standard output. Note that while LocalProcess 
implements AProcess for host OS, some other derived class might 
choose to e.g. represent a process on remote system.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4;H0; &]
[s5;:AProcess`:`:Kill`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Kill]()_`=_[@3 0]&]
[s2;%% Terminates running process.&]
[s3;%% &]
[s4; &]
[s5;:AProcess`:`:IsRunning`(`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsRunning]()_`=_[@3 0
]&]
[s2;%% Returns true if process is running.&]
[s3; &]
[s4; &]
[s5;:AProcess`:`:Write`(String`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Write]([_^String^ S
tring]_[*@3 s])_`=_[@3 0]&]
[s2;%% Sends a text to standard input of process.&]
[s3;%% &]
[s4; &]
[s5;:AProcess`:`:Read`(String`&`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* Read]([_^String^ S
tring][@(0.0.255) `&]_[*@3 s])_`=_[@3 0]&]
[s2;%% Reads data from standard output combined with standard error 
output. Returns true if there process was running or there are 
more data to be read.&]
[s3;%% &]
[s4; &]
[s5;:AProcess`:`:Read2`(String`&`,String`&`): [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* Read2]([_^String^ String][@(0.0.255) `&]_[*@3 os], 
[_^String^ String][@(0.0.255) `&]_[*@3 es])&]
[s2;%% Reads data separately from standard output combined and from 
standard error output. Returns true if there process was running 
or there are more data to be read. Usually, implementing class 
has to be in special mode for this to work (e.g. LocalProcess 
must be started with Start2).&]
[s3;%% &]
[s4; &]
[s5;:AProcess`:`:GetExitCode`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetExitCode]()_`=
_[@3 0]&]
[s2;%% Returns an exit code of terminated process.&]
[s3; &]
[s4; &]
[s5;:AProcess`:`:GetExitMessage`(`): [@(0.0.255) virtual] [_^String^ String]_[* GetExitMess
age]()&]
[s2;%% In case of error, might return its text description.&]
[s3; &]
[s4; &]
[s5;:AProcess`:`:CloseRead`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* CloseRead]()&]
[s2;%% Closes handle to process output.&]
[s3; &]
[s4; &]
[s5;:AProcess`:`:CloseWrite`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* CloseWrite]()&]
[s2;%% Closes handle to process input. This is useful e.g.  in case 
that process waits for the end of input data before processing 
them and providing output.&]
[s3; &]
[s4; &]
[s5;:AProcess`:`:Detach`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Detach]()&]
[s2;%% Disconnects AProcess instance from running child process. 
Process continues to run while AProcess instance can be destructed.&]
[s3; &]
[s4; &]
[s5;:AProcess`:`:Get`(`): [_^String^ String]_[* Get]()&]
[s2;%% This utility inline function calls Read and returns the standard 
output of process. If anything went wrong, returns String`::GetVoid().&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 LocalProcess]]}}&]
[s3; &]
[s1;:LocalProcess`:`:class: [@(0.0.255)3 class][3 _][*3 LocalProcess][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 AProcess]&]
[s9;%% This class implements a child process of host operating system.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3;%% &]
[s5;:LocalProcess`:`:Start`(const char`*`,const char`*`): [@(0.0.255) bool]_[* Start]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 cmdline], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 envptr
]_`=_NULL)&]
[s2;%% Starts a new process defined by [%-*@3 cmdline], [%-*@3 envptr 
]can provide a new environment for the process, if NULL, then 
the new process inherits caller`'s environment.&]
[s3;%% &]
[s4; &]
[s5;:LocalProcess`:`:Start2`(const char`*`,const char`*`): [@(0.0.255) bool]_[* Start2]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 cmdline], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 envptr
]_`=_NULL)&]
[s2;%% Starts a new process defined by [%-*@3 cmdline], [%-*@3 envptr 
]can provide a new environment for the process, if NULL, then 
the new process inherits caller`'s environment. This variant 
activates mode when standard output and standard error output 
are read separately using Read2 method.&]
[s3;%% &]
[s4; &]
[s5;:LocalProcess`:`:ConvertCharset`(bool`): [_^LocalProcess^ LocalProcess][@(0.0.255) `&
]_[* ConvertCharset]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:LocalProcess`:`:NoConvertCharset`(`): [_^LocalProcess^ LocalProcess][@(0.0.255) `&]_
[* NoConvertCharset]()&]
[s2;%% Determines LocalProcess should convert encoding from system 
to application one. Default setting is true.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s4;H0; &]
[s5;:LocalProcess`:`:LocalProcess`(`): [* LocalProcess]()&]
[s2;%% Default constructor.&]
[s3; &]
[s4; &]
[s5;:LocalProcess`:`:LocalProcess`(const char`*`,const char`*`): [* LocalProcess]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 cmdline], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 envptr
]_`=_NULL)&]
[s2;%% Equivalent of default constructor and then invoking Start([%-*@3 cmdline][%- , 
][%-*@3 envptr]).&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Global functions related to LocalProcess]]}}&]
[s3; &]
[s5;:Sys`(const char`*`,String`&`,bool`): [@(0.0.255) int]_[* Sys]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 cmd], [_^String^ String][@(0.0.255) `&]_[*@3 out], [@(0.0.255) bool]_[*@3 convert
charset]_`=_[@(0.0.255) true])&]
[s2;%% Runs process defined by [%-*@3 cmd] command line, returns its 
standard output in [%-*@3 output].and its exit code as return value. 
If there was error invoking [%-*@3 cmd], returns `-1. If [%-*@3 convertcharset] 
is true, output is converted from system character encoding to 
application encoding.&]
[s3;%% &]
[s4; &]
[s5;:Sys`(const char`*`,bool`): [_^String^ String]_[* Sys]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 cmd], [@(0.0.255) bool]_[*@3 convertcharset]_`=_[@(0.0.255) true])&]
[s2;%% Runs process defined by [%-*@3 cmd] command line. If [%-*@3 cmd] 
was executed successfully and returned zero exit code, returns 
its standard output, otherwise returns String`::GetVoid(). If 
[%-*@3 convertcharset] is true, output is converted from system 
character encoding to application encoding.&]
[s3; &]
[s0; ]]