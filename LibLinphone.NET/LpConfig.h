/*
LpConfig.h
Copyright (C) 2015  Belledonne Communications, Grenoble, France
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#pragma once

#include "LinphoneCore.h"

namespace Linphone
{
	namespace Core
	{
		ref class LinphoneCore;

		/// <summary>
		/// Object used to manipulate a configuration file.
		/// The format of the configuration file is a .ini like format:
		/// - sections are defined in []
		/// - each section contains a sequence of key=value pairs.
		/// <example>
		/// [sound]
		/// echocanceler=1
		/// playback_dev=ALSA: Default device
		/// 
		/// [video]
		/// enabled=1
		/// </example>
		/// </summary>
		public ref class LpConfig sealed
		{
		public:

            // Constructor
            LpConfig(::LpConfig *config);
            LpConfig(System::String^ configPath, System::String^ factoryConfigPath);

			/// <summary>
			/// Gets a boolean configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="defaultValue">The value to return if the configuration item is not found</param>
			/// <returns>The value of the configuration item or defaultValue if the item is not found</returns>
			bool GetBool(System::String^ section, System::String^ key, bool defaultValue);

			/// <summary>
			/// Sets a boolean configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="value">The value to set for the configuration item</param>
			void SetBool(System::String^ section, System::String^ key, bool value);

			/// <summary>
			/// Gets an integer configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="defaultValue">The value to return if the configuration item is not found</param>
			/// <returns>The value of the configuration item or defaultValue if the item is not found</returns>
			int GetInt(System::String^ section, System::String^ key, int defaultValue);

			/// <summary>
			/// Sets an integer configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="value">The value to set for the configuration item</param>
			void SetInt(System::String^ section, System::String^ key, int value);

			/// <summary>
			/// Gets a 64 bit integer configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="defaultValue">The value to return if the configuration item is not found</param>
			/// <returns>The value of the configuration item or defaultValue if the item is not found</returns>
			int64 GetInt64(System::String^ section, System::String^ key, int64 defaultValue);

			/// <summary>
			/// Sets a 64 bit integer configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="value">The value to set for the configuration item</param>
			void SetInt64(System::String^ section, System::String^ key, int64 value);

			/// <summary>
			/// Gets a float configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="defaultValue">The value to return if the configuration item is not found</param>
			/// <returns>The value of the configuration item or defaultValue if the item is not found</returns>
			float GetFloat(System::String^ section, System::String^ key, float defaultValue);

			/// <summary>
			/// Sets a float configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="value">The value to set for the configuration item</param>
			void SetFloat(System::String^ section, System::String^ key, float value);

			/// <summary>
			/// Gets a string configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="defaultValue">The value to return if the configuration item is not found</param>
			/// <returns>The value of the configuration item or defaultValue if the item is not found</returns>
			System::String^ GetString(System::String^ section, System::String^ key, System::String^ defaultValue);

			/// <summary>
			/// Sets a string configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="value">The value to set for the configuration item</param>
			void SetString(System::String^ section, System::String^ key, System::String^ value);

			/// <summary>
			/// Gets an integer range configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="defaultValue">The value to return if the configuration item is not found</param>
			/// <returns>The value of the configuration item or defaultValue if the item is not found</returns>
            cli::array<int>^ GetRange(System::String^ section, System::String^ key, cli::array<int>^ defaultValue);

			/// <summary>
			/// Sets an integer range configuration item.
			/// </summary>
			/// <param name="section">The name of the configuration section where the item is located</param>
			/// <param name="key">The configuration item name</param>
			/// <param name="value">The value to set for the configuration item</param>
            void SetRange(System::String^ section, System::String^ key, cli::array<int>^ value);

            /// <summary>
            /// Get inner LpConfig pointer
            /// </summary>
            ::LpConfig* GetConfigPtr();

		private:
			~LpConfig();

			::LpConfig *config;
		};
	}
}