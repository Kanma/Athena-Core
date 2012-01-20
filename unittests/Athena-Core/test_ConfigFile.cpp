#include <UnitTest++.h>
#include <Athena-Core/Data/ConfigFile.h>
#include <Athena-Core/Data/FileDataStream.h>
#include <Athena-Core/Utils/StringConverter.h>
#include <Athena-Core/Log/LogManager.h>
#include <Athena-Core/Log/XMLLogListener.h>
#include "Utils.h"

using namespace Athena;
using namespace Athena::Data;
using namespace Athena::Utils;
using namespace Athena::Log;


struct Loader
{
	Loader()
	: bLoaded(false)
	{
		bLoaded = cfgFile.load(getFilePath("TestConfigFile.cfg"));
	}

	~Loader()
	{
		if (bLoaded)
			cfgFile.unload();
	}


	ConfigFile cfgFile;
	bool		bLoaded;
};



struct General
{
	General()
	{
	}

	~General()
	{
	}


	bool saveAndLoad()
	{
		if (!cfgFile.save(getFilePath("TestConfigFile2.cfg")))
			return false;

		cfgFile.unload();

		if (!cfgFile.load(getFilePath("TestConfigFile2.cfg")))
			return false;

		return true;
	}


	ConfigFile cfgFile;
};



SUITE(ConfigFile_Loading)
{
	TEST_FIXTURE(Loader, LoadingSuccessful)
	{
		CHECK(bLoaded);
	}


	TEST_FIXTURE(Loader, RightSectionsCount)
	{
		CHECK_EQUAL(8, cfgFile.getNbSections());
	}


	TEST_FIXTURE(Loader, UnloadSuccessful)
	{
		cfgFile.unload();
		CHECK_EQUAL(0, cfgFile.getNbSections());
	}


	TEST_FIXTURE(Loader, NoSectionParameter)
	{
		std::string value;
		
		CHECK(cfgFile.getParameterValue("", "orphan1", value));
		CHECK(!value.empty());
	}


	TEST_FIXTURE(Loader, BuiltinTypesString)
	{
		std::string value;
		
		CHECK(cfgFile.selectSection("Builtin types"));
		CHECK(cfgFile.getParameterValue("string", value));
		CHECK(!value.empty());
	}


	TEST_FIXTURE(Loader, BuiltinTypesUnsignedInt)
	{
		unsigned int value = 0;
		
		CHECK(cfgFile.selectSection("Builtin types"));
		CHECK(cfgFile.getParameterValue("uint", value));
		CHECK(value != 0);
	}


	TEST_FIXTURE(Loader, BuiltinTypesInt)
	{
		int value = 0;
		
		CHECK(cfgFile.selectSection("Builtin types"));
		CHECK(cfgFile.getParameterValue("int", value));
		CHECK(value != 0);
	}


	TEST_FIXTURE(Loader, BuiltinTypesFloat)
	{
		float value = 0.0f;
		
		CHECK(cfgFile.selectSection("Builtin types"));
		CHECK(cfgFile.getParameterValue("float", value));
		CHECK(value > 0.0f);
	}


	TEST_FIXTURE(Loader, BuiltinTypesBool1)
	{
		bool value = false;
		
		CHECK(cfgFile.selectSection("Builtin types"));
		CHECK(cfgFile.getParameterValue("bool1", value));
		CHECK_EQUAL(true, value);
	}


	TEST_FIXTURE(Loader, BuiltinTypesBool2)
	{
		bool value = true;
		
		CHECK(cfgFile.selectSection("Builtin types"));
		CHECK(cfgFile.getParameterValue("bool2", value));
		CHECK_EQUAL(false, value);
	}


	TEST_FIXTURE(Loader, Parsing)
	{
		std::string value;

		cfgFile.selectSection("Parsing");

		for (unsigned int i = 1; i <= 9; ++i)
		{
			std::string strName = "val" + StringConverter::toString(i);
			std::string strExpected = "OK " + StringConverter::toString(i);

			CHECK(cfgFile.getParameterValue(strName, value));
			CHECK_EQUAL(strExpected, value);
		}
	}


	TEST_FIXTURE(Loader, EmptyParameters)
	{
		cfgFile.selectSection("Empty parameters");


		std::string value = "notEmpty";

		CHECK(cfgFile.getParameterValue("empty1", value));
		CHECK(value.empty());

		
		value = "notEmpty";

		CHECK(cfgFile.getParameterValue("empty2", value));
		CHECK(value.empty());
	}


	TEST_FIXTURE(Loader, RightSectionsWithSameNameCount)
	{
		CHECK_EQUAL(3, cfgFile.getNbSections("Clone category"));
	}


	TEST_FIXTURE(Loader, SectionsWithSameNameIndexing)
	{
		unsigned int value;

		for (unsigned int i = 0; i < 3; ++i)
		{
			CHECK(cfgFile.selectSection("Clone category", i));
			CHECK(cfgFile.getParameterValue("index", value));
			CHECK_EQUAL(i, value);
		}

		CHECK(!cfgFile.selectSection("Clone category", 3));
	}


	TEST_FIXTURE(Loader, SectionsEnumeration)
	{
		std::string strName = "";

		for (unsigned int i = 0; i < 8; ++i)
			CHECK(cfgFile.selectNextSection(strName));

		CHECK(!cfgFile.selectNextSection(strName));
	}


	TEST_FIXTURE(Loader, RightParametersWithSameNameCount)
	{
		cfgFile.selectSection("Clone params");
		CHECK_EQUAL(5, cfgFile.getNbParameters("clone"));
	}


	TEST_FIXTURE(Loader, ParametersWithSameNameEnumeration)
	{
		std::string strValue = "";

		cfgFile.selectSection("Clone params");
		
		for (unsigned int i = 0; i < 5; ++i)
		{
			std::string strExpected = StringConverter::toString(i);

			CHECK(cfgFile.getNextParameterValue("clone", strValue));
			CHECK_EQUAL(strExpected, strValue);
		}

		CHECK(!cfgFile.getNextParameterValue("clone", strValue));
	}


	TEST_FIXTURE(Loader, FindExistingParameter)
	{
		cfgFile.selectSection("Clone params");
		CHECK(cfgFile.hasParameter("notAClone"));
	}


	TEST_FIXTURE(Loader, DontFindNonExistingParameter)
	{
		cfgFile.selectSection("Clone params");
		CHECK(!cfgFile.hasParameter("unknown"));
	}


	TEST_FIXTURE(Loader, FindExistingParameterWithSectionName)
	{
		std::string value = "";

		CHECK(cfgFile.getParameterValue("Builtin types", "bool1", value));
		CHECK_EQUAL("true", value);
	}


	TEST_FIXTURE(Loader, ParametersEnumeration)
	{
		std::string name;
		std::string value;

		cfgFile.selectSection("Parsing");

		for (unsigned int i = 1; i <= 9; ++i)
		{
			std::string strExpectedName = "val" + StringConverter::toString(i);
			std::string strExpectedValue = "OK " + StringConverter::toString(i);

			CHECK(cfgFile.getNextParameter(name, value));
			CHECK_EQUAL(strExpectedName, name);
			CHECK_EQUAL(strExpectedValue, value);
		}

		CHECK(!cfgFile.getNextParameter(name, value));
	}
}


SUITE(ConfigFile_Creation)
{
	TEST_FIXTURE(General, SaveEmptyFile)
	{
		CHECK(saveAndLoad());
		CHECK_EQUAL(0, cfgFile.getNbSections());
	}


	TEST_FIXTURE(General, AddSections)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addSection("Section 2");
		cfgFile.addSection("Section 3");

		CHECK(saveAndLoad());
		CHECK_EQUAL(3, cfgFile.getNbSections());


		std::string strName = "";

		CHECK(cfgFile.selectNextSection(strName));
        CHECK_EQUAL("Section 1", strName);

		CHECK(cfgFile.selectNextSection(strName));
        CHECK_EQUAL("Section 2", strName);

		CHECK(cfgFile.selectNextSection(strName));
        CHECK_EQUAL("Section 3", strName);

		CHECK(!cfgFile.selectNextSection(strName));
	}


	TEST_FIXTURE(General, InsertSection)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addSection("Section 2");
		cfgFile.addSection("Section 3");
		
		CHECK(cfgFile.selectSection("Section 1"));

		cfgFile.addSection("Section 4", true);

		CHECK(saveAndLoad());
		CHECK_EQUAL(4, cfgFile.getNbSections());


		std::string strName = "";

		CHECK(cfgFile.selectNextSection(strName));
        CHECK_EQUAL("Section 1", strName);

		CHECK(cfgFile.selectNextSection(strName));
        CHECK_EQUAL("Section 4", strName);

		CHECK(cfgFile.selectNextSection(strName));
        CHECK_EQUAL("Section 2", strName);

		CHECK(cfgFile.selectNextSection(strName));
        CHECK_EQUAL("Section 3", strName);

		CHECK(!cfgFile.selectNextSection(strName));
	}


	TEST_FIXTURE(General, AddParameters)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addParameter("Parameter", "value1");
		cfgFile.addParameter("Parameter", "value2");
		cfgFile.addParameter("Parameter", "value3");


		CHECK(saveAndLoad());

		cfgFile.selectSection("Section 1");

		CHECK_EQUAL(3, cfgFile.getNbParameters());
	}


	TEST_FIXTURE(General, AddString)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addParameter("Parameter", "value");

		CHECK(saveAndLoad());

		
		cfgFile.selectSection("Section 1");

		std::string value = "";

		CHECK(cfgFile.getParameterValue("Parameter", value));
		CHECK_EQUAL(std::string("value"), value);
	}


	TEST_FIXTURE(General, AddUnsignedInt)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addParameter("Parameter", (unsigned int) 10);

		CHECK(saveAndLoad());

		
		cfgFile.selectSection("Section 1");

		unsigned int value = 0;

		CHECK(cfgFile.getParameterValue("Parameter", value));
		CHECK_EQUAL(10, value);
	}


	TEST_FIXTURE(General, AddInt)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addParameter("Parameter", (int) -10);

		CHECK(saveAndLoad());

		
		cfgFile.selectSection("Section 1");

		int value = 0;

		CHECK(cfgFile.getParameterValue("Parameter", value));
		CHECK_EQUAL(-10, value);
	}


	TEST_FIXTURE(General, AddFloat)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addParameter("Parameter", 56.3f);

		CHECK(saveAndLoad());

		
		cfgFile.selectSection("Section 1");

		float value = 0.0f;

		CHECK(cfgFile.getParameterValue("Parameter", value));
		CHECK_CLOSE(56.3f, value, 0.0001f);
	}


	TEST_FIXTURE(General, AddBool)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addParameter("Parameter1", true);
		cfgFile.addParameter("Parameter2", false);

		CHECK(saveAndLoad());

		
		cfgFile.selectSection("Section 1");

		bool value;

		CHECK(cfgFile.getParameterValue("Parameter1", value));
		CHECK_EQUAL(true, value);

		CHECK(cfgFile.getParameterValue("Parameter2", value));
		CHECK_EQUAL(false, value);
	}


	TEST_FIXTURE(General, AddParameterWithoutSection)
	{
		cfgFile.addSection("");
		cfgFile.addParameter("Parameter", "value");

		CHECK(saveAndLoad());

		std::string value = "";

		CHECK(cfgFile.getParameterValue("", "Parameter", value));
		CHECK_EQUAL(std::string("value"), value);
	}


	TEST_FIXTURE(General, ModifyParameter)
	{
		cfgFile.addSection("Section 1");
		cfgFile.addParameter("Parameter", "value1");

		CHECK(saveAndLoad());

		
		cfgFile.selectSection("Section 1");

		std::string value = "";

		CHECK(cfgFile.getParameterValue("Parameter", value));
		CHECK_EQUAL(std::string("value1"), value);

		CHECK(cfgFile.modifyParameter("Parameter", "value2"));

		CHECK(saveAndLoad());

		
		cfgFile.selectSection("Section 1");

		value = "";

		CHECK(cfgFile.getParameterValue("Parameter", value));
		CHECK_EQUAL(std::string("value2"), value);
	}

}


SUITE(ConfigFile_Others)
{
	TEST(ReloadingSuccessful)
	{
		ConfigFile cfgFile;

		CHECK(cfgFile.load(getFilePath("TestConfigFile.cfg")));
		
		unsigned int uiNbSections = cfgFile.getNbSections();

		cfgFile.addSection("New");
		CHECK_EQUAL(uiNbSections + 1, cfgFile.getNbSections());

		CHECK(cfgFile.reload());
		CHECK_EQUAL(uiNbSections, cfgFile.getNbSections());
	}


	TEST(ReloadingOfUnknownFileFail)
	{
        // Disable error output on stderr
        LogManager logManager;
        logManager.addListener(new XMLLogListener("test_log.xml"), true);


		ConfigFile cfgFile;
		
        FileDataStream stream(getFilePath("TestConfigFile.cfg"));

		CHECK(cfgFile.load(&stream));
		CHECK(!cfgFile.reload());
	}
}
