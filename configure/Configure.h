#ifndef BASE_CONFIGURE_H
#define BASE_CONFIGURE_H

#include "../Singleton.h"
#include "../luatinker/lua_tinker.h"
#include "../utils/String.h"

namespace base
{
	class Configure : public Singleton<Configure>
	{
		friend class Singleton<Configure>;
	public:
		void SetConfigureFile(const char * filename)
		{
			lua_tinker::dofile(m_lua, filename);
		}
		
		template<typename T>
		T GetConfigure(const string& configure)
		{
			assert(nullptr != m_lua);
			vector<string> vec = utils::split_string(configure, '.');
			uint8_t size = vec.size();
			if(size == 1)
				return lua_tinker::get<T>(m_lua, configure.c_str());
			else if(size > 1)
			{
				lua_tinker::table source(m_lua);
				string src = vec[0];
				source.set("tbl", lua_tinker::get<lua_tinker::table>(m_lua, src.c_str()));
				for(int i = 1; i < size - 1; ++i)
				{
					src = vec[i];					
					lua_tinker::table tmp = source.get<lua_tinker::table>("tbl");
					source.set("tbl", tmp.get<lua_tinker::table>(src.c_str()));
				}
				src = vec[size-1];
				lua_tinker::table target = source.get<lua_tinker::table>("tbl");
				return target.get<T>(src.c_str());
			}
		}

	private:
		Configure()
		{
			m_lua = lua_open();
			luaopen_base(m_lua);
		}
	private:
		lua_State * m_lua;
	};
}

#define sConfigure (*Configure::Instance())
#endif