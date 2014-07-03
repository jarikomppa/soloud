module DL
  SEM = Mutex.new

  def set_callback_internal(proc_entry, addr_entry, argc, ty, abi = nil, &cbp)
    raise(ArgumentError, "arity should not be less than 0.") if (argc < 0)
    addr = nil

    SEM.synchronize{
      ary = proc_entry[ty]
      (0...MAX_CALLBACK).each{|n|
        idx = (n * DLSTACK_SIZE) + argc
        if( ary[idx].nil? )
          ary[idx] = cbp
          addr = addr_entry[ty][idx]
          break
        end
      }
    }

    addr
  end

  def set_cdecl_callback(ty, argc, &cbp)
    set_callback_internal(CdeclCallbackProcs, CdeclCallbackAddrs, argc, ty, &cbp)
  end

  def set_stdcall_callback(ty, argc, &cbp)
    set_callback_internal(StdcallCallbackProcs, StdcallCallbackAddrs, argc, ty, &cbp)
  end

  def remove_callback_internal(proc_entry, addr_entry, addr, ctype = nil)    
    index = nil
    if( ctype )
      addr_entry[ctype].each_with_index{|xaddr, idx|
        if( xaddr == addr )
          index = idx
        end
      }
    else
      addr_entry.each{|ty,entry|
        entry.each_with_index{|xaddr, idx|
          if( xaddr == addr )
            index = idx
          end
        }
      }
    end
    if( index and proc_entry[ctype][index] )
      proc_entry[ctype][index] = nil
      return true
    else
      return false
    end
  end

  def remove_cdecl_callback(addr, ctype = nil)
    remove_callback_internal(CdeclCallbackProcs, CdeclCallbackAddrs, addr, ctype)
  end

  def remove_stdcall_callback(addr, ctype = nil)
    remove_callback_internal(StdcallCallbackProcs, StdcallCallbackAddrs, addr, ctype)
  end

  alias set_callback set_cdecl_callback
  alias remove_callback remove_cdecl_callback

  class Stack
    def self.[](*types)
      new(types)
    end

    def initialize(types)
      parse_types(types)
    end

    def size()
      @size
    end

    def types()
      @types
    end

    def pack(ary)
      case SIZEOF_VOIDP
      when SIZEOF_LONG
        ary.pack(@template).unpack('l!*')
      when SIZEOF_LONG_LONG
        ary.pack(@template).unpack('q*')
      else
        raise(RuntimeError, "sizeof(void*)?")
      end
    end

    def unpack(ary)
      case SIZEOF_VOIDP
      when SIZEOF_LONG
        ary.pack('l!*').unpack(@template)
      when SIZEOF_LONG_LONG
        ary.pack('q*').unpack(@template)
      else
        raise(RuntimeError, "sizeof(void*)?")
      end
    end

    private

    def align(addr, align)
      d = addr % align
      if( d == 0 )
        addr
      else
        addr + (align - d)
      end
    end

    ALIGN_MAP = {
      TYPE_VOIDP => ALIGN_VOIDP,
      TYPE_CHAR  => ALIGN_VOIDP,
      TYPE_SHORT => ALIGN_VOIDP,
      TYPE_INT   => ALIGN_VOIDP,
      TYPE_LONG  => ALIGN_VOIDP,
      TYPE_FLOAT => ALIGN_FLOAT,
      TYPE_DOUBLE => ALIGN_DOUBLE,
    }

    PACK_MAP = {
      TYPE_VOIDP => ((SIZEOF_VOIDP == SIZEOF_LONG_LONG)? "q" : "l!"),
      TYPE_CHAR  => "c",
      TYPE_SHORT => "s!",
      TYPE_INT   => "i!",
      TYPE_LONG  => "l!",
      TYPE_FLOAT => "f",
      TYPE_DOUBLE => "d",
    }

    SIZE_MAP = {
      TYPE_VOIDP => SIZEOF_VOIDP,
      TYPE_CHAR  => SIZEOF_CHAR,
      TYPE_SHORT => SIZEOF_SHORT,
      TYPE_INT   => SIZEOF_INT,
      TYPE_LONG  => SIZEOF_LONG,
      TYPE_FLOAT => SIZEOF_FLOAT,
      TYPE_DOUBLE => SIZEOF_DOUBLE,
    }
    if defined?(TYPE_LONG_LONG)
      ALIGN_MAP[TYPE_LONG_LONG] = ALIGN_LONG_LONG
      PACK_MAP[TYPE_LONG_LONG] = "q"
      SIZE_MAP[TYPE_LONG_LONG] = SIZEOF_LONG_LONG
    end

    def parse_types(types)
      @types = types
      @template = ""
      addr      = 0
      types.each{|t|
        addr = add_padding(addr, ALIGN_MAP[t])
        @template << PACK_MAP[t]
        addr += SIZE_MAP[t]
      }
      addr = add_padding(addr, ALIGN_MAP[SIZEOF_VOIDP])
      if( addr % SIZEOF_VOIDP == 0 )
        @size = addr / SIZEOF_VOIDP
      else
        @size = (addr / SIZEOF_VOIDP) + 1
      end
    end

    def add_padding(addr, align)
      orig_addr = addr
      addr = align(orig_addr, align)
      d = addr - orig_addr
      if( d > 0 )
        @template << "x#{d}"
      end
      addr
    end
  end

  module PackInfo
    ALIGN_MAP = {
      TYPE_VOIDP => ALIGN_VOIDP,
      TYPE_CHAR  => ALIGN_CHAR,
      TYPE_SHORT => ALIGN_SHORT,
      TYPE_INT   => ALIGN_INT,
      TYPE_LONG  => ALIGN_LONG,
      TYPE_FLOAT => ALIGN_FLOAT,
      TYPE_DOUBLE => ALIGN_DOUBLE,
      -TYPE_CHAR  => ALIGN_CHAR,
      -TYPE_SHORT => ALIGN_SHORT,
      -TYPE_INT   => ALIGN_INT,
      -TYPE_LONG  => ALIGN_LONG,
    }

    PACK_MAP = {
      TYPE_VOIDP => ((SIZEOF_VOIDP == SIZEOF_LONG_LONG) ? "q" : "l!"),
      TYPE_CHAR  => "c",
      TYPE_SHORT => "s!",
      TYPE_INT   => "i!",
      TYPE_LONG  => "l!",
      TYPE_FLOAT => "f",
      TYPE_DOUBLE => "d",
      -TYPE_CHAR  => "c",
      -TYPE_SHORT => "s!",
      -TYPE_INT   => "i!",
      -TYPE_LONG  => "l!",
    }

    SIZE_MAP = {
      TYPE_VOIDP => SIZEOF_VOIDP,
      TYPE_CHAR  => SIZEOF_CHAR,
      TYPE_SHORT => SIZEOF_SHORT,
      TYPE_INT   => SIZEOF_INT,
      TYPE_LONG  => SIZEOF_LONG,
      TYPE_FLOAT => SIZEOF_FLOAT,
      TYPE_DOUBLE => SIZEOF_DOUBLE,
      -TYPE_CHAR  => SIZEOF_CHAR,
      -TYPE_SHORT => SIZEOF_SHORT,
      -TYPE_INT   => SIZEOF_INT,
      -TYPE_LONG  => SIZEOF_LONG,
    }
    if defined?(TYPE_LONG_LONG)
      ALIGN_MAP[TYPE_LONG_LONG] = ALIGN_MAP[-TYPE_LONG_LONG] = ALIGN_LONG_LONG
      PACK_MAP[TYPE_LONG_LONG] = PACK_MAP[-TYPE_LONG_LONG] = "q"
      SIZE_MAP[TYPE_LONG_LONG] = SIZE_MAP[-TYPE_LONG_LONG] = SIZEOF_LONG_LONG
    end

    def align(addr, align)
      d = addr % align
      if( d == 0 )
        addr
      else
        addr + (align - d)
      end
    end
    module_function :align
  end

  class Packer
    include PackInfo

    def self.[](*types)
      new(types)
    end

    def initialize(types)
      parse_types(types)
    end

    def size()
      @size
    end

    def pack(ary)
      case SIZEOF_VOIDP
      when SIZEOF_LONG
        ary.pack(@template)
      when SIZEOF_LONG_LONG
        ary.pack(@template)
      else
        raise(RuntimeError, "sizeof(void*)?")
      end
    end

    def unpack(ary)
      case SIZEOF_VOIDP
      when SIZEOF_LONG
        ary.join().unpack(@template)
      when SIZEOF_LONG_LONG
        ary.join().unpack(@template)
      else
        raise(RuntimeError, "sizeof(void*)?")
      end
    end

    private

    def parse_types(types)
      @template = ""
      addr     = 0
      types.each{|t|
        orig_addr = addr
        if( t.is_a?(Array) )
          addr = align(orig_addr, ALIGN_MAP[TYPE_VOIDP])
        else
          addr = align(orig_addr, ALIGN_MAP[t])
        end
        d = addr - orig_addr
        if( d > 0 )
          @template << "x#{d}"
        end
        if( t.is_a?(Array) )
          @template << (PACK_MAP[t[0]] * t[1])
          addr += (SIZE_MAP[t[0]] * t[1])
        else
          @template << PACK_MAP[t]
          addr += SIZE_MAP[t]
        end
      }
      addr = align(addr, ALIGN_MAP[TYPE_VOIDP])
      @size = addr
    end
  end

  module ValueUtil
    def unsigned_value(val, ty)
      case ty.abs
      when TYPE_CHAR
        [val].pack("c").unpack("C")[0]
      when TYPE_SHORT
        [val].pack("s!").unpack("S!")[0]
      when TYPE_INT
        [val].pack("i!").unpack("I!")[0]
      when TYPE_LONG
        [val].pack("l!").unpack("L!")[0]
      when TYPE_LONG_LONG
        [val].pack("q").unpack("Q")[0]
      else
        val
      end
    end

    def signed_value(val, ty)
      case ty.abs
      when TYPE_CHAR
        [val].pack("C").unpack("c")[0]
      when TYPE_SHORT
        [val].pack("S!").unpack("s!")[0]
      when TYPE_INT
        [val].pack("I!").unpack("i!")[0]
      when TYPE_LONG
        [val].pack("L!").unpack("l!")[0]
      when TYPE_LONG_LONG
        [val].pack("Q").unpack("q")[0]
      else
        val
      end
    end

    def wrap_args(args, tys, funcs, &block)
      result = []
      tys ||= []
      args.each_with_index{|arg, idx|
        result.push(wrap_arg(arg, tys[idx], funcs, &block))
      }
      result
    end

    def wrap_arg(arg, ty, funcs = [], &block)
        funcs ||= []
        case arg
        when nil
          return 0
        when CPtr
          return arg.to_i
        when IO
          case ty
          when TYPE_VOIDP
            return CPtr[arg].to_i
          else
            return arg.to_i
          end
        when Function
          if( block )
            arg.bind_at_call(&block)
            funcs.push(arg)
          elsif !arg.bound?
            raise(RuntimeError, "block must be given.")
          end
          return arg.to_i
        when String
          if( ty.is_a?(Array) )
            return arg.unpack('C*')
          else
            case SIZEOF_VOIDP
            when SIZEOF_LONG
              return [arg].pack("p").unpack("l!")[0]
            when SIZEOF_LONG_LONG
              return [arg].pack("p").unpack("q")[0]
            else
              raise(RuntimeError, "sizeof(void*)?")
            end
          end
        when Float, Integer
          return arg
        when Array
          if( ty.is_a?(Array) ) # used only by struct
            case ty[0]
            when TYPE_VOIDP
              return arg.collect{|v| Integer(v)}
            when TYPE_CHAR
              if( arg.is_a?(String) )
                return val.unpack('C*')
              end
            end
            return arg
          else
            return arg
          end
        else
          if( arg.respond_to?(:to_ptr) )
            return arg.to_ptr.to_i
          else
            begin
              return Integer(arg)
            rescue
              raise(ArgumentError, "unknown argument type: #{arg.class}")
            end
          end
        end
    end
  end

  class Function < Object
    include DL
    include ValueUtil

    def initialize cfunc, argtypes, abi = nil, &block
      @cfunc = cfunc
      @stack = Stack.new(argtypes.collect{|ty| ty.abs})
      if( @cfunc.ctype < 0 )
        @cfunc.ctype = @cfunc.ctype.abs
        @unsigned = true
      else
        @unsigned = false
      end
      if block_given?
        bind(&block)
      end
    end

    def to_i()
      @cfunc.to_i
    end

    def name
      @cfunc.name
    end

    def call(*args, &block)
      funcs = []
      _args = wrap_args(args, @stack.types, funcs, &block)
      r = @cfunc.call(@stack.pack(_args))
      funcs.each{|f| f.unbind_at_call()}
      return wrap_result(r)
    end

    def wrap_result(r)
      case @cfunc.ctype
      when TYPE_VOIDP
        r = CPtr.new(r)
      else
        if( @unsigned )
          r = unsigned_value(r, @cfunc.ctype)
        end
      end
      r
    end

    def bind(&block)
      if( !block )
        raise(RuntimeError, "block must be given.")
      end
      if( @cfunc.ptr == 0 )
        cb = Proc.new{|*args|
          ary = @stack.unpack(args)
          @stack.types.each_with_index{|ty, idx|
            case ty
            when TYPE_VOIDP
              ary[idx] = CPtr.new(ary[idx])
            end
          }
          r = block.call(*ary)
          wrap_arg(r, @cfunc.ctype, [])
        }
        case @cfunc.calltype
        when :cdecl
          @cfunc.ptr = set_cdecl_callback(@cfunc.ctype, @stack.size, &cb)
        when :stdcall
          @cfunc.ptr = set_stdcall_callback(@cfunc.ctype, @stack.size, &cb)
        else
          raise(RuntimeError, "unsupported calltype: #{@cfunc.calltype}")
        end
        if( @cfunc.ptr == 0 )
          raise(RuntimeException, "can't bind C function.")
        end
      end
    end

    def unbind()
      if( @cfunc.ptr != 0 )
        case @cfunc.calltype
        when :cdecl
          remove_cdecl_callback(@cfunc.ptr, @cfunc.ctype)
        when :stdcall
          remove_stdcall_callback(@cfunc.ptr, @cfunc.ctype)
        else
          raise(RuntimeError, "unsupported calltype: #{@cfunc.calltype}")
        end
        @cfunc.ptr = 0
      end
    end

    def bound?()
      @cfunc.ptr != 0
    end

    def bind_at_call(&block)
      bind(&block)
    end

    def unbind_at_call()
    end
  end

  class TempFunction < Function
    def bind_at_call(&block)
      bind(&block)
    end

    def unbind_at_call()
      unbind()
    end
  end

  class CarriedFunction < Function
    def initialize(cfunc, argtypes, n)
      super(cfunc, argtypes)
      @carrier = []
      @index = n
      @mutex = Mutex.new
    end

    def create_carrier(data)
      ary = []
      userdata = [ary, data]
      @mutex.lock()
      @carrier.push(userdata)
      return dlwrap(userdata)
    end

    def bind_at_call(&block)
      userdata = @carrier[-1]
      userdata[0].push(block)
      bind{|*args|
        ptr = args[@index]
        if( !ptr )
          raise(RuntimeError, "The index of userdata should be lower than #{args.size}.")
        end
        userdata = dlunwrap(Integer(ptr))
        args[@index] = userdata[1]
        userdata[0][0].call(*args)
      }
      @mutex.unlock()
    end
  end

  class CStruct
    def CStruct.entity_class()
      CStructEntity
    end
  end

  class CUnion
    def CUnion.entity_class()
      CUnionEntity
    end
  end

  module CStructBuilder
    def create(klass, types, members)
      new_class = Class.new(klass){
        define_method(:initialize){|addr|
          @entity = klass.entity_class.new(addr, types)
          @entity.assign_names(members)
        }
        define_method(:to_ptr){ @entity }
        define_method(:to_i){ @entity.to_i }
        members.each{|name|
          define_method(name){ @entity[name] }
          define_method(name + "="){|val| @entity[name] = val }
        }
      }
      size = klass.entity_class.size(types)
      new_class.module_eval(<<-EOS, __FILE__, __LINE__+1)
        def new_class.size()
          #{size}
        end
        def new_class.malloc()
          addr = DL.malloc(#{size})
          new(addr)
        end
      EOS
      return new_class
    end
    module_function :create
  end

  class CStructEntity < CPtr
    include PackInfo
    include ValueUtil

    def CStructEntity.malloc(types, func = nil)
      addr = DL.malloc(CStructEntity.size(types))
      CStructEntity.new(addr, types, func)
    end

    def CStructEntity.size(types)
      offset = 0
      max_align = 0
      types.each_with_index{|t,i|
        orig_offset = offset
        if( t.is_a?(Array) )
          align = PackInfo::ALIGN_MAP[t[0]]
          offset = PackInfo.align(orig_offset, align)
          size = offset - orig_offset
          offset += (PackInfo::SIZE_MAP[t[0]] * t[1])
        else
          align = PackInfo::ALIGN_MAP[t]
          offset = PackInfo.align(orig_offset, align)
          size = offset - orig_offset
          offset += PackInfo::SIZE_MAP[t]
        end
        if (max_align < align)
          max_align = align
        end
      }
      offset = PackInfo.align(offset, max_align)
      offset
    end

    def initialize(addr, types, func = nil)
      set_ctypes(types)
      super(addr, @size, func)
    end

    def assign_names(members)
      @members = members
    end

    def set_ctypes(types)
      @ctypes = types
      @offset = []
      offset = 0
      max_align = 0
      types.each_with_index{|t,i|
        orig_offset = offset
        if( t.is_a?(Array) )
          align = ALIGN_MAP[t[0]]
        else
          align = ALIGN_MAP[t]
        end
        offset = PackInfo.align(orig_offset, align)
        @offset[i] = offset
        if( t.is_a?(Array) )
          offset += (SIZE_MAP[t[0]] * t[1])
        else
          offset += SIZE_MAP[t]
        end
        if (max_align < align)
          max_align = align
        end
      }
      offset = PackInfo.align(offset, max_align)
      @size = offset
    end

    def [](name)
      idx = @members.index(name)
      if( idx.nil? )
        raise(ArgumentError, "no such member: #{name}")
      end
      ty = @ctypes[idx]
      if( ty.is_a?(Array) )
        r = super(@offset[idx], SIZE_MAP[ty[0]] * ty[1])
      else
        r = super(@offset[idx], SIZE_MAP[ty.abs])
      end
      packer = Packer.new([ty])
      val = packer.unpack([r])
      case ty
      when Array
        case ty[0]
        when TYPE_VOIDP
          val = val.collect{|v| CPtr.new(v)}
        end
      when TYPE_VOIDP
        val = CPtr.new(val[0])
      else
        val = val[0]
      end
      if( ty.is_a?(Integer) && (ty < 0) )
        return unsigned_value(val, ty)
      elsif( ty.is_a?(Array) && (ty[0] < 0) )
        return val.collect{|v| unsigned_value(v,ty[0])}
      else
        return val
      end
    end

    def []=(name, val)
      idx = @members.index(name)
      if( idx.nil? )
        raise(ArgumentError, "no such member: #{name}")
      end
      ty  = @ctypes[idx]
      packer = Packer.new([ty])
      val = wrap_arg(val, ty, [])
      buff = packer.pack([val].flatten())
      super(@offset[idx], buff.size, buff)
      if( ty.is_a?(Integer) && (ty < 0) )
        return unsigned_value(val, ty)
      elsif( ty.is_a?(Array) && (ty[0] < 0) )
        return val.collect{|v| unsigned_value(v,ty[0])}
      else
        return val
      end
    end

    def to_s()
      super(@size)
    end
  end

  class CUnionEntity < CStructEntity
    include PackInfo

    def CUnionEntity.malloc(types, func=nil)
      addr = DL.malloc(CUnionEntity.size(types))
      CUnionEntity.new(addr, types, func)
    end

    def CUnionEntity.size(types)
      size   = 0
      types.each_with_index{|t,i|
        if( t.is_a?(Array) )
          tsize = PackInfo::SIZE_MAP[t[0]] * t[1]
        else
          tsize = PackInfo::SIZE_MAP[t]
        end
        if( tsize > size )
          size = tsize
        end
      }
    end

    def set_ctypes(types)
      @ctypes = types
      @offset = []
      @size   = 0
      types.each_with_index{|t,i|
        @offset[i] = 0
        if( t.is_a?(Array) )
          size = SIZE_MAP[t[0]] * t[1]
        else
          size = SIZE_MAP[t]
        end
        if( size > @size )
          @size = size
        end
      }
    end
  end

  module CParser
    def parse_struct_signature(signature, tymap=nil)
      if( signature.is_a?(String) )
        signature = signature.split(/\s*,\s*/)
      end
      mems = []
      tys  = []
      signature.each{|msig|
        tks = msig.split(/\s+(\*)?/)
        ty = tks[0..-2].join(" ")
        member = tks[-1]

        case ty
        when /\[(\d+)\]/
          n = $1.to_i
          ty.gsub!(/\s*\[\d+\]/,"")
          ty = [ty, n]
        when /\[\]/
          ty.gsub!(/\s*\[\]/, "*")
        end

        case member
        when /\[(\d+)\]/
          ty = [ty, $1.to_i]
          member.gsub!(/\s*\[\d+\]/,"")
        when /\[\]/
          ty = ty + "*"
          member.gsub!(/\s*\[\]/, "")
        end

        mems.push(member)
        tys.push(parse_ctype(ty,tymap))
      }
      return tys, mems
    end

    def parse_signature(signature, tymap=nil)
      tymap ||= {}
      signature = signature.gsub(/\s+/, " ").strip
      case signature
      when /^([\w@\*\s]+)\(([\w\*\s\,\[\]]*)\)$/
        ret = $1
        (args = $2).strip!
        ret = ret.split(/\s+/)
        args = args.split(/\s*,\s*/)
        func = ret.pop
        if( func =~ /^\*/ )
          func.gsub!(/^\*+/,"")
          ret.push("*")
        end
        ret  = ret.join(" ")
        return [func, parse_ctype(ret, tymap), args.collect{|arg| parse_ctype(arg, tymap)}]
      else
        raise(RuntimeError,"can't parse the function prototype: #{signature}")
      end
    end

    def parse_ctype(ty, tymap=nil)
      tymap ||= {}
      case ty
      when Array
        return [parse_ctype(ty[0], tymap), ty[1]]
      when "void"
        return TYPE_VOID
      when "char"
        return TYPE_CHAR
      when "unsigned char"
        return  -TYPE_CHAR
      when "short"
        return TYPE_SHORT
      when "unsigned short"
        return -TYPE_SHORT
      when "int"
        return TYPE_INT
      when "unsigned int", 'uint'
        return -TYPE_INT
      when "long"
        return TYPE_LONG
      when "unsigned long"
        return -TYPE_LONG
      when "long long"
        if( defined?(TYPE_LONG_LONG) )
          return TYPE_LONG_LONG
        else
          raise(RuntimeError, "unsupported type: #{ty}")
        end
      when "unsigned long long"
        if( defined?(TYPE_LONG_LONG) )
          return -TYPE_LONG_LONG
        else
          raise(RuntimeError, "unsupported type: #{ty}")
        end
      when "float"
        return TYPE_FLOAT
      when "double"
        return TYPE_DOUBLE
      when /\*/, /\[\s*\]/
        return TYPE_VOIDP
      else
        if( tymap[ty] )
          return parse_ctype(tymap[ty], tymap)
        else
          raise(DLError, "unknown type: #{ty}")
        end
      end
    end
  end

  class CompositeHandler
    def initialize(handlers)
      @handlers = handlers
    end

    def handlers()
      @handlers
    end

    def sym(symbol)
      @handlers.each{|handle|
        if( handle )
          begin
            addr = handle.sym(symbol)
            return addr
          rescue DLError
          end
        end
      }
      return nil
    end

    def [](symbol)
      sym(symbol)
    end
  end

  module Importer
    include DL
    include CParser
    extend Importer

    def dlload(*libs)
      handles = libs.collect{|lib|
        case lib
        when nil
          nil
        when Handle
          lib
        when Importer
          lib.handlers
        else
          begin
            DL.dlopen(lib)
          rescue DLError
            raise(DLError, "can't load #{lib}")
          end
        end
      }.flatten()
      @handler = CompositeHandler.new(handles)
      @func_map = {}
      @type_alias = {}
    end

    def typealias(alias_type, orig_type)
      @type_alias[alias_type] = orig_type
    end

    def sizeof(ty)
      case ty
      when String
        ty = parse_ctype(ty, @type_alias).abs()
        case ty
        when TYPE_CHAR
          return SIZEOF_CHAR
        when TYPE_SHORT
          return SIZEOF_SHORT
        when TYPE_INT
          return SIZEOF_INT
        when TYPE_LONG
          return SIZEOF_LONG
        when TYPE_LONG_LONG
          return SIZEOF_LONG_LON
        when TYPE_FLOAT
          return SIZEOF_FLOAT
        when TYPE_DOUBLE
          return SIZEOF_DOUBLE
        when TYPE_VOIDP
          return SIZEOF_VOIDP
        else
          raise(DLError, "unknown type: #{ty}")
        end
      when Class
        if( ty.instance_methods().include?(:to_ptr) )
          return ty.size()
        end
      end
      return CPtr[ty].size()
    end

    def parse_bind_options(opts)
      h = {}
      while( opt = opts.shift() )
        case opt
        when :stdcall, :cdecl
          h[:call_type] = opt
        when :carried, :temp, :temporal, :bind
          h[:callback_type] = opt
          h[:carrier] = opts.shift()
        else
          h[opt] = true
        end
      end
      h
    end
    private :parse_bind_options

    def extern(signature, *opts)
      symname, ctype, argtype = parse_signature(signature, @type_alias)
      opt = parse_bind_options(opts)
      f = import_function(symname, ctype, argtype, opt[:call_type])
      name = symname.gsub(/@.+/,'')
      @func_map[name] = f
      
      begin
        /^(.+?):(\d+)/ =~ caller.first
        file, line = $1, $2.to_i
      rescue
        file, line = __FILE__, __LINE__+3
      end
      module_eval(<<-EOS, file, line)
        def #{name}(*args, &block)
          @func_map['#{name}'].call(*args,&block)
        end
      EOS
      module_function(name)
      f
    end

    def bind(signature, *opts, &blk)
      name, ctype, argtype = parse_signature(signature, @type_alias)
      h = parse_bind_options(opts)
      case h[:callback_type]
      when :bind, nil
        f = bind_function(name, ctype, argtype, h[:call_type], &blk)
      when :temp, :temporal
        f = create_temp_function(name, ctype, argtype, h[:call_type])
      when :carried
        f = create_carried_function(name, ctype, argtype, h[:call_type], h[:carrier])
      else
        raise(RuntimeError, "unknown callback type: #{h[:callback_type]}")
      end
      @func_map[name] = f
      begin
        /^(.+?):(\d+)/ =~ caller.first
        file, line = $1, $2.to_i
      rescue
        file, line = __FILE__, __LINE__+3
      end
      module_eval(<<-EOS, file, line)
        def #{name}(*args,&block)
          @func_map['#{name}'].call(*args,&block)
        end
      EOS
      module_function(name)
      f
    end

    def struct(signature)
      tys, mems = parse_struct_signature(signature, @type_alias)
      DL::CStructBuilder.create(CStruct, tys, mems)
    end

    def union(signature)
      tys, mems = parse_struct_signature(signature, @type_alias)
      DL::CStructBuilder.create(CUnion, tys, mems)
    end

    def [](name)
      @func_map[name]
    end

    def create_value(ty, val=nil)
      s = struct([ty + " value"])
      ptr = s.malloc()
      if( val )
        ptr.value = val
      end
      return ptr
    end
    alias value create_value

    def import_value(ty, addr)
      s = struct([ty + " value"])
      ptr = s.new(addr)
      return ptr
    end

    def handler
      @handler or raise "call dlload before importing symbols and functions"
    end

    def import_symbol(name)
      addr = handler.sym(name)
      if( !addr )
        raise(DLError, "cannot find the symbol: #{name}")
      end
      CPtr.new(addr)
    end

    def import_function(name, ctype, argtype, call_type = nil)
      addr = handler.sym(name)
      if( !addr )
        raise(DLError, "cannot find the function: #{name}()")
      end
      Function.new(CFunc.new(addr, ctype, name, call_type || :cdecl), argtype)
    end

    def bind_function(name, ctype, argtype, call_type = nil, &block)
      f = Function.new(CFunc.new(0, ctype, name, call_type || :cdecl), argtype)
      f.bind(&block)
      f
    end

    def create_temp_function(name, ctype, argtype, call_type = nil)
      TempFunction.new(CFunc.new(0, ctype, name, call_type || :cdecl), argtype)
    end

    def create_carried_function(name, ctype, argtype, call_type = nil, n = 0)
      CarriedFunction.new(CFunc.new(0, ctype, name, call_type || :cdecl), argtype, n)
    end
  end
end