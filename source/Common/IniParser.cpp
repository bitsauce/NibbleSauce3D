//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>

BEGIN_SAUCE_NAMESPACE

IniFile::IniFile(string path) :
	m_path(path)
{
	util::toAbsoluteFilePath(path);

	if(!util::fileExists(path))
		return;

	FileReader *reader = new FileReader(path);

	string comment, sectionName;
	while(!reader->isEOF())
	{
		string line = reader->readLine();
		if(line.empty())
			continue;
		line = line.substr(line.find_first_not_of(' '), string::npos);

		if(line.front() == ';')
		{
			comment += line.substr(1, string::npos);
		}
		else if(line.front() == '[' && line.back() == ']')
		{
			sectionName = line.substr(1, line.size()-2);

			Section &section = m_sections[sectionName];
			section.comment = comment;
			comment.clear();
		}
		else if(line.find('=') != string::npos && m_sections.find(sectionName) != m_sections.end())
		{
			string keyName = line.substr(0, line.find('='));
			string keyValue = line.substr(line.find('=') + 1, string::npos);

			Key &key = m_sections[sectionName].keys[keyName];
			key.value = keyValue;
			key.comment = comment;
			comment.clear();
		}
	}

	delete reader;
}

string IniFile::getValue(const string &sec, const string &key) const
{
	if(isSection(sec))
	{
		const Section &section = m_sections.at(sec);
		if(section.keys.find(key) != section.keys.end())
		{
			return section.keys.at(key).value;
		}
	}
	return "";
}

void IniFile::setValue(const string &sec, const string &key, const string &value)
{
	Section &section = m_sections[sec];
	section.keys[key].value = value;
}

bool IniFile::isSection(const string &sec) const
{
	return m_sections.find(sec) != m_sections.end();
}

void IniFile::save()
{
	stringstream out;
	for(map<string, Section>::iterator sectionItr = m_sections.begin(); sectionItr != m_sections.end(); ++sectionItr)
	{
		Section &section = sectionItr->second;
		if(!section.comment.empty()) {
			out << ";" << section.comment << endl;
		}
		out << "[" << sectionItr->first << "]" << endl;

		for(map<string, Key>::iterator keyItr = section.keys.begin(); keyItr != section.keys.end(); ++keyItr)
		{
			if(!keyItr->second.comment.empty()) {
				out << ";" << keyItr->second.comment << endl;
			}
			out << keyItr->first << "=" << keyItr->second.value << endl;
		}

		out << endl;
	}
	
	FileSystem::WriteFile(m_path, out.str());
}

END_SAUCE_NAMESPACE
