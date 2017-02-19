using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zetBlock
    {
    internal class BinGroup
        {
        private ICollection<Bin> Bins { get; set; }
        public int Amount { get { return Bins.Count (); } }

        private int m_id;
        public int Id
            {
            get
                {
                return m_id;
                }
            set
                {
                m_id = value;
                foreach (var bin in Bins)
                    {
                    bin.Id = value;
                    }
                }
            }

        private int m_height;
        public int Height
            {
            get
                {
                return m_height;
                }
            set
                {
                m_height = value;
                foreach (var bin in Bins)
                    {
                    bin.Height = value;
                    }
                }
            }

        private int m_width;
        public int Width
            {
            get
                {
                return m_width;
                }
            set
                {
                m_width = value;
                foreach (var bin in Bins)
                    {
                    bin.Width = value;
                    }
                }
            }

        public BinGroup(int id, int width, int height, int ammount)
            {
            m_id = id;
            m_height = height;
            m_width = width;
            Bins = new List<Bin> (ammount);
            for (int i = 0; i < ammount; i++)
                {
                Bins.Add (new Bin (id, width, height));
                }
            }
        }
    }
